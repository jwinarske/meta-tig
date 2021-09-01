DESCRIPTION = "Telegraf"
SUMMARY = "Telegraf is the open source server agent to help you collect metrics from your stacks, sensors and systems."
HOMEPAGE = "https://www.influxdata.com/time-series-platform/telegraf/"

INSANE_SKIP:${PN}:append = " already-stripped"

SRC_URI:append = " ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'file://volatiles.telegraf.conf', 'file://volatiles.99_telegraf', d)}"

# --> we need a more complete ps for init script
RDEPENDS:${PN} += "procps"
# <-- we need a more complete ps for init script

# --> user/group
inherit useradd

# create telegraf group
# --system                  create a system account
GROUPADD_PARAM:${PN} = "--system telegraf"

# create telegraf user
# --system                  create a system account
# --gid GROUP               name or ID of the primary group of the new
#                           account
USERADD_PARAM:${PN} += "telegraf \
--system \
--no-create-home \
--home-dir /var/run/${BPN} \
--shell /bin/false \
--gid telegraf \
"
##########################################################
# e.g.
#
# id telegraf
# uid=997(telegraf) gid=996(telegraf) groups=996(telegraf)
#
# cat /etc/group | grep telegraf
# telegraf:x:996:
#
# cat /etc/passwd | grep telegraf
# telegraf:x:997:996::/var/run/telegraf:/bin/false
#
# cat /etc/shadow | grep telegraf
# telegraf:!:18766:0:99999:7:::
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
    #${localstatedir} = /var

    # /etc
    install -d ${D}${sysconfdir}/logrotate.d
    install -d ${D}${sysconfdir}/telegraf
    install -d ${D}${sysconfdir}/telegraf/telegraf.d


    # armv7 has this file, x86-64 not 
    # @@@ TODO: fix it
    if [ -f  ${S}/etc/logrotate.d/telegraf ]; then
    install -m 0644 ${S}/etc/logrotate.d/telegraf ${D}${sysconfdir}/logrotate.d/
    fi

    # x86-64 and armv7 have telegraf.conf at different places
    if [ -f ${S}/etc/telegraf/telegraf.conf ]; then
      install -m 0644 ${S}/etc/telegraf/telegraf.conf ${D}${sysconfdir}/telegraf/
    else
      install -m 0644 ${S}/telegraf.conf ${D}${sysconfdir}/telegraf/
    fi

    # /usr/bin
    install -d ${D}${bindir}

    # arm has a bin dir
    if [ -d ${S}/usr/bin/ ]; then
    install -m 0755 ${S}/usr/bin/telegraf ${D}${bindir}/
    else
    # x86-64 old does not
    install -m 0755 ${S}/telegraf ${D}${bindir}/
    fi

    # /usr/lib
    if [ -d ${S}/usr/lib ]; then
      # only for armv7 - in older versions?

      # Only install the script if 'sysvinit' is in DISTRO_FEATURES
      # systemd would be the other choice
      if ${@bb.utils.contains('DISTRO_FEATURES','sysvinit','true','false',d)}; then
          install -d ${D}${sysconfdir}/init.d/
          sed -i 's,#! /usr/bin/env bash,#! /bin/sh,g' ${S}/usr/lib/telegraf/scripts/init.sh
          # SIGTERM -> TERM
          sed -i 's,SIGTERM, TERM,g' ${S}/usr/lib/telegraf/scripts/init.sh
          # SIGHUP -> HUP
          sed -i 's,SIGHUP, HUP,g' ${S}/usr/lib/telegraf/scripts/init.sh
          install -D -m 0755 ${S}/usr/lib/telegraf/scripts/init.sh ${D}${sysconfdir}/init.d/telegraf

          # volatile log files
          install -d ${D}${sysconfdir}/default/volatiles
          install -m 644 ${WORKDIR}/volatiles.99_telegraf ${D}${sysconfdir}/default/volatiles/99_telegraf
      fi

      # Only install the script if 'systemd' is in DISTRO_FEATURES
      # systemd
      if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd','true','false',d)}; then
          install -d ${D}${systemd_unitdir}/system
	  install -m 0644 ${S}/usr/lib/telegraf/scripts/telegraf.service ${D}${systemd_unitdir}/system/telegraf.service

           # --> if we have DISTRO_FEATURE tig we want to control the startup sequence
           # 1) influxdb
           # 2) telegraf
           # 3) grafana
           if ${@bb.utils.contains('DISTRO_FEATURES', 'tig','true','false',d)}; then
              # --> various modifications to telegraf.service in order to make sure it's 2)
              #    --> add
              #        after this line: After=network.target
              #        this line:       Before=grafana.service
              sed -i '/^After=network.target/a Before=grafana.service' ${D}${systemd_unitdir}/system/telegraf.service
              #    <-- add
              #    --> replace
              #        this line: After=network.target
              #        with this: After=network.target influxdb.service
              sed -i 's/After=network.target/After=network.target influxdb.service/' ${D}${systemd_unitdir}/system/telegraf.service
              #    <-- replace

              #    --> add
              #        after this line: WantedBy=multi-user.target
              #        this line:       Wants=influxdb.service
              sed -i '/^WantedBy=multi-user.target/a Wants=influxdb.service' ${D}${systemd_unitdir}/system/telegraf.service
              #    <-- add
              #    --> replace
              #        this line: WantedBy=multi-user.target
              #        with this: WantedBy=multi-user.target grafana.service
              sed -i 's/WantedBy=multi-user.target/WantedBy=multi-user.target grafana.service/' ${D}${systemd_unitdir}/system/telegraf.service
              #    <-- replace
              # <-- various modifications to telegraf.service in order to make sure it's 2)
           fi
           # <-- if we have DISTRO_FEATURE tig we want to control the startup sequence

          # volatile log files
          install -d ${D}${sysconfdir}/tmpfiles.d
          install -m 644 ${WORKDIR}/volatiles.telegraf.conf ${D}${sysconfdir}/tmpfiles.d/telegraf.conf
      fi
    fi # -d ${S}/usr/lib

    # /var - for /var/log/telegraf/telegraf.log
    # for some reason for DISTRO=venus the install does not work - exclude it
    #${@ bb.utils.contains("DISTRO", "venus", "", "install -d ${D}${localstatedir}/log/telegraf", d)}
    #install -d ${D}${localstatedir}/log/telegraf
}

# --> systemd service
# please note, that above
#   we already copy files depeding on sysvinit/systemd
#   we already inherited systemd
SYSTEMD_AUTO_ENABLE = "enable"
# disable for manual testing
# e.g. on target:
# systemctl start telegraf.service
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
# script has a runlevel of: 99
# starts in initlevels:     2 3 4 5
# stops  in initlevels: 0 1         6
INITSCRIPT_PARAMS:${PN} = "start 94 2 3 4 5 . stop 94 0 1 6 ."
# <-- sysvinit scripts
