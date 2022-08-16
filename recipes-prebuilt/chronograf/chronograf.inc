DESCRIPTION = "Chronograf"
SUMMARY = "Chronograf is the complete interface for the InfluxDB 1.x Platform"
HOMEPAGE = "https://www.influxdata.com/time-series-platform/chronograf/"

INSANE_SKIP:${PN}:append = " already-stripped"
INSANE_SKIP:${PN}:append = " ldflags"

# --> user/group
inherit useradd

# create chronograf group
# --system                  create a system account
GROUPADD_PARAM:${PN} = "--system chronograf"

# create chronograf user
# --system                  create a system account
# --gid GROUP               name or ID of the primary group of the new
#                           account
USERADD_PARAM:${PN} += "chronograf \
--system \
--no-create-home \
--home-dir /var/run/${BPN} \
--shell /bin/false \
--gid chronograf \
"

# USERADD_PACKAGES specifies the output packages
# which include custom users/groups.
USERADD_PACKAGES = "${PN}"
# <-- user/group

# for the time being only systemd is supported
SRC_URI:append = " ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'file://volatiles.chronograf.conf', 'file://volatiles.99_chronograf', d)}"

SRC_URI:append = " file://etc-default-chronograf"

do_install() {
    #${sysconfdir} = /etc
    #${bindir} = /usr/bin
    #${libdir} = /usr/lib
    #${localstatedir} = /var
    #${datadir} = /usr/share

    # /etc
    install -d ${D}${sysconfdir}/logrotate.d
    install -m 0644 ${S}/etc/logrotate.d/chronograf ${D}${sysconfdir}/logrotate.d

    # needed at least by the sysvinit script -> /etc/default/chronograf (where chronograf is a file)
    install -d ${D}${sysconfdir}/default
    install -m 0644 -o chronograf -g chronograf ${WORKDIR}/etc-default-chronograf ${D}${sysconfdir}/default/chronograf

    # /usr/bin
    install -d ${D}${bindir}
    install -m 0755 ${S}/usr/bin/* ${D}${bindir}

    # */

    # /usr/lib
    #install -d ${D}${libdir}/chronograf/scripts
    #install -m 0644 ${S}/usr/lib/chronograf/scripts/* ${D}${libdir}/chronograf/scripts

    # */

    # --> systemd
    # Only install the script if 'systemd' is in DISTRO_FEATURES
    if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd','true','false',d)}; then
       install -d ${D}${systemd_unitdir}/system
       # we created a chronograf user/group, so we don't need to replace it with root
       # sed -i 's/User=chronograf/User=root/g' ${S}/usr/lib/chronograf/scripts/chronograf.service
       # sed -i 's/Group=chronograf/Group=root/g' ${S}/usr/lib/chronograf/scripts/chronograf.service
       install -m 0644 ${S}/usr/lib/chronograf/scripts/chronograf.service ${D}${systemd_unitdir}/system
    fi
    # <-- systemd

    # --> sysvinit
       # Only install the script if 'sysvinit' is in DISTRO_FEATURES
       # systemd would be the other choice
       if ${@bb.utils.contains('DISTRO_FEATURES','sysvinit','true','false',d)}; then
           install -d ${D}${sysconfdir}/init.d/
           # --> /bin/sh instead of /bin/bash
           sed -i 's,#!/bin/bash,#! /bin/sh,g' ${S}/usr/lib/chronograf/scripts/init.sh
           # <-- /bin/sh instead of /bin/bash
           install -D -m 0755 ${S}/usr/lib/chronograf/scripts/init.sh ${D}${sysconfdir}/init.d/chronograf

           # volatile log files
           install -d ${D}${sysconfdir}/default/volatiles
           install -m 644 ${WORKDIR}/volatiles.99_chronograf ${D}${sysconfdir}/default/volatiles/99_chronograf
       fi 
     # <-- sysvinit    

    # /usr/share
    install -d ${D}${datadir}/chronograf/canned
    install -m 0644 ${S}/usr/share/chronograf/canned/* ${D}${datadir}/chronograf/canned
    # */
    install -d ${D}${datadir}/chronograf/protoboards
    install -m 0644 ${S}/usr/share/chronograf/protoboards/* ${D}${datadir}/chronograf/protoboards
    # */
    install -d ${D}${datadir}/chronograf/resources

    # /var/lib
    install -dm 755 -o chronograf -g chronograf ${D}${localstatedir}/lib/chronograf
    #install -d ${D}${localstatedir}/lib/chronograf
    #install -d ${D}${localstatedir}/log/chronograf

}

inherit update-rc.d systemd

# --> sysvinit scripts
# please note, that above
#   we already copy files depeding on sysvinit/systemd
#   we already inherited update-rc.d
INITSCRIPT_PACKAGES = "${PN}"
INITSCRIPT_NAME:${PN} = "${PN}"
# script has a runlevel of: 93
# starts in initlevels:     2 3 4 5
# stops  in initlevels: 0 1         6
INITSCRIPT_PARAMS:${PN} = "start 99 2 3 4 5 . stop 99 0 1 6 ."
# <-- sysvinit scripts

# --> systemd service
# please note, that above
#   we already copy files depeding on sysvinit/systemd
#   we already inherited systemd
SYSTEMD_AUTO_ENABLE = "enable"
# disable for manual testing
# e.g. on target:
# systemctl start chronograf.service
#SYSTEMD_AUTO_ENABLE = "disable"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "${PN}.service"
# <-- systemd service

