DESCRIPTION = "InfluxDB"
SUMMARY = "InfluxDB is a time series database designed to handle high write and query loads."
HOMEPAGE = "https://www.influxdata.com/products/influxdb-overview/"

INSANE_SKIP:${PN}:append = " already-stripped"

SRC_URI:append = " ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'file://volatiles.influxdb.conf', 'file://volatiles.99_influxdb', d)}"

# --> user/group
inherit useradd

# create influxdb group
# --system                  create a system account
GROUPADD_PARAM:${PN} = "--system influxdb"

# create influxdb user
# --system                  create a system account
# --gid GROUP               name or ID of the primary group of the new
#                           account
USERADD_PARAM:${PN} += "influxdb \
--system \
--no-create-home \
--home-dir /var/run/${BPN} \
--shell /bin/false \
--gid influxdb \
"
##########################################################
# e.g.
#
# id influxdb
# uid=997(influxdb) gid=996(influxdb) groups=996(influxdb)
#
# cat /etc/group | grep influxdb
# influxdb:x:996:
#
# cat /etc/passwd | grep influxdb
# influxdb:x:997:996::/var/run/influxdb:/bin/false
#
# cat /etc/shadow | grep influxdb
# influxdb:!:18809:0:99999:7:::
##########################################################

# USERADD_PACKAGES specifies the output packages
# which include custom users/groups.
USERADD_PACKAGES = "${PN}"
# <-- user/group

inherit update-rc.d systemd

do_install() {
    #${sysconfdir} = /etc
    #${bindir} = /usr/bin
    #${libdir} = /usr/lib
    #${datadir} = /usr/share
    #${localstatedir} = /var

    # /etc
    install -d ${D}${sysconfdir}/influxdb
    install -d ${D}${sysconfdir}/logrotate.d

    # x86-64 and armv7 have influxdb.conf at different places
    if [ -f ${S}/etc/influxdb/influxdb.conf ]; then
      install -m 0644 ${S}/etc/influxdb/influxdb.conf ${D}${sysconfdir}/influxdb/
    else
      install -m 0644 ${S}/influxdb.conf ${D}${sysconfdir}/influxdb/ 
    fi 
 
    # armv7 has this file, x86-64 not 
    # @@@ TODO: fix it
    if [ -f  ${S}/etc/logrotate.d/influxdb ]; then
    install -m 0644 ${S}/etc/logrotate.d/influxdb ${D}${sysconfdir}/logrotate.d/
    fi

    # /usr/bin
    install -d ${D}${bindir}

    # arm has a bin dir
    if [ -d ${S}/usr/bin/ ]; then
    install -m 0755 ${S}/usr/bin/* ${D}${bindir}/
    # */
    else
    # x86-64 does not
    install -m 0755 ${S}/influx ${D}${bindir}/
    install -m 0755 ${S}/influxd ${D}${bindir}/
    install -m 0755 ${S}/influx_inspect ${D}${bindir}/
    install -m 0755 ${S}/influx_stress ${D}${bindir}/
    install -m 0755 ${S}/influx_tsm ${D}${bindir}/
    fi

    # /usr/lib
    if [ -d ${S}/usr/lib ]; then
       # only form armv7

       # Only install the script if 'sysvinit' is in DISTRO_FEATURES
       # systemd would be the other choice
       if ${@bb.utils.contains('DISTRO_FEATURES','sysvinit','true','false',d)}; then
           install -d ${D}${sysconfdir}/init.d/
           # --> /bin/sh instead of /bin/bash
           sed -i 's,#!/bin/bash,#! /bin/sh,g' ${S}/usr/lib/influxdb/scripts/init.sh
           # <-- /bin/sh instead of /bin/bash
           # --> we need /var/lib/influxdb u:influxdb/g:influxdb
           #     not sure this is the right place, we'll see when we test systemd as well
           #     I added it further down both for sys-v and influxdb, so I guess this can be removed
           #sed -i '/^# Max open files/i# /var/lib/influxdb dir\nVARLIBDIR=/var/lib/influxdb\nif [ ! -d "$VARLIBDIR" ]; then\n    mkdir -p $VARLIBDIR\n    chown $USER:$GROUP $VARLIBDIR\nfi\n' ${S}/usr/lib/influxdb/scripts/init.sh
           # <-- we need /var/lib/influxdb u:influxdb/g:influxdb
           install -D -m 0755 ${S}/usr/lib/influxdb/scripts/init.sh ${D}${sysconfdir}/init.d/influxdb
 
           # volatile log files
           install -d ${D}${sysconfdir}/default/volatiles
           install -m 644 ${WORKDIR}/volatiles.99_influxdb ${D}${sysconfdir}/default/volatiles/99_influxdb
       fi # sysvinit


       # Only install the script if 'systemd' is in DISTRO_FEATURES
       # systemd
       if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd','true','false',d)}; then
           install -d ${D}${systemd_unitdir}/system

           # we created influxdb user/group so no hacking needed here
           install -m 0644 ${S}/usr/lib/influxdb/scripts/influxdb.service ${D}${systemd_unitdir}/system/influxdb.service

           # --> if we have DISTRO_FEATURE tig we want to control the startup sequence
           # 1) influxdb
           # 2) telegraf
           # 3) grafana
           if ${@bb.utils.contains('DISTRO_FEATURES', 'tig','true','false',d)}; then
              # --> various modifications to influxdb.service in order to make sure it's 1)
              #    --> add
              #        after this line: After=network-online.target
              #        this line:       Before=telegraf.service grafana.service
              sed -i '/^After=network-online.target/a Before=telegraf.service grafana.service' ${D}${systemd_unitdir}/system/influxdb.service
              #    <-- add
              #    --> replace
              #        this line: WantedBy=multi-user.target
              #        with this: WantedBy=multi-user.target telegraf.service
              sed -i 's/WantedBy=multi-user.target/WantedBy=multi-user.target telegraf.service/' ${D}${systemd_unitdir}/system/influxdb.service
              #    <-- replace
              # <-- various modifications to influxdb.service in order to make sure it's 1)
           fi
           # <-- if we have DISTRO_FEATURE tig we want to control the startup sequence

           # volatile log files
           install -d ${D}${sysconfdir}/tmpfiles.d
           install -m 644 ${WORKDIR}/volatiles.influxdb.conf ${D}${sysconfdir}/tmpfiles.d/influxdb.conf
       fi # systemd

    # only form armv7
    #install -d ${D}${systemd_unitdir}/system
    #sed -i 's/User=influxdb/User=root/g' ${S}/usr/lib/influxdb/scripts/influxdb.service
    #sed -i 's/Group=influxdb/Group=root/g' ${S}/usr/lib/influxdb/scripts/influxdb.service
    #install -m 0644 ${S}/usr/lib/influxdb/scripts/influxdb.service ${D}${systemd_unitdir}/system
    #install -m 0644 ${S}/usr/lib/influxdb/scripts/influxdb.service ${D}${libdir}/influxdb/scripts/
    #install -m 0644 ${S}/usr/lib/influxdb/scripts/init.sh ${D}${libdir}/influxdb/scripts/
    fi

    # /usr/share - man pages
    install -d ${D}${datadir}/man/man1
    install -m 0644 ${S}/usr/share/man/man1/* ${D}${datadir}/man/man1/
    # */

    # we need /var/lib/influxdb u:influxdb/g:influxdb
    mkdir -p ${D}${localstatedir}/lib/influxdb
    chown influxdb:influxdb ${D}${localstatedir}/lib/influxdb

    # not sure about this one /lib/influxdb??
    #install -d ${D}${localstatedir}/lib/influxdb
    #install -d ${D}${localstatedir}/log/influxdb
}

# --> systemd service
# please note, that above
#   we already copy files depeding on sysvinit/systemd
#   we already inherited systemd
SYSTEMD_AUTO_ENABLE = "enable"
# disable for manual testing
# e.g. on target:
# systemctl start influxdb.service
#SYSTEMD_AUTO_ENABLE = "disable"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "${PN}.service"
# <-- systemd service

# --> sysvinit scripts
# please note, that above
#   we already copy files depeding on sysvinit/systemd
#   we already inherited update-rc.d
INITSCRIPT_PACKAGES = "${PN}"
INITSCRIPT_NAME:${PN} = "${PN}"
# script has a runlevel of: 93
# starts in initlevels:     2 3 4 5
# stops  in initlevels: 0 1         6
INITSCRIPT_PARAMS:${PN} = "start 93 2 3 4 5 . stop 93 0 1 6 ."
# <-- sysvinit scripts
