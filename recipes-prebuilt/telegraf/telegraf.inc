DESCRIPTION = "Telegraf"
SUMMARY = "Telegraf is the open source server agent to help you collect metrics from your stacks, sensors and systems."
HOMEPAGE = "https://www.influxdata.com/time-series-platform/telegraf/"

INSANE_SKIP_${PN}_append = "already-stripped"

# --> we need a more complete ps for init script
RDEPENDS_${PN} += "procps"
# <-- we need a more complete ps for init script

# --> user/group
inherit useradd

# create telegraf group
# --system                  create a system account
GROUPADD_PARAM_${PN} = "--system telegraf"

# create telegraf user
# --system                  create a system account
# --gid GROUP               name or ID of the primary group of the new
#                           account
USERADD_PARAM_${PN} += "telegraf \
                        --system \
                        --gid telegraf \
                       "
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
    # only for armv7 - in older versions

    # Only install the script if 'sysvinit' is in DISTRO_FEATURES
    # systemd would be the other choice
    if ${@bb.utils.contains('DISTRO_FEATURES','sysvinit','true','false',d)}; then
        install -d ${D}${sysconfdir}/init.d/
        sed -i 's,#! /usr/bin/env bash,#! /bin/sh,g' ${S}/usr/lib/telegraf/scripts/init.sh
	# @@@TODO: create telegraf user/group and don't replace by root
	#sed -i 's/USER=telegraf/USER=root/g' ${S}/usr/lib/telegraf/scripts/init.sh
	#sed -i 's/GROUP=telegraf/GROUP=root/g' ${S}/usr/lib/telegraf/scripts/init.sh
        install -D -m 0755 ${S}/usr/lib/telegraf/scripts/init.sh ${D}${sysconfdir}/init.d/telegraf
    fi

    # Only install the script if 'systemd' is in DISTRO_FEATURES
    # systemd
    if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd','true','false',d)}; then
        install -d ${D}${systemd_unitdir}/system
        # we have a telegraf user - so we can run it as telegraf user
	#sed -i 's/User=telegraf/User=root/g' ${S}/usr/lib/telegraf/scripts/telegraf.service
	install -m 0644 ${S}/usr/lib/telegraf/scripts/telegraf.service ${D}${systemd_unitdir}/system/telegraf.service
    fi

    #install -d ${D}${systemd_unitdir}/system
    #sed -i 's/User=telegraf/User=root/g' ${S}/usr/lib/telegraf/scripts/telegraf.service
    #install -m 0644 ${S}/usr/lib/telegraf/scripts/telegraf.service ${D}${systemd_unitdir}/system

    #install -m 0644 ${S}/usr/lib/telegraf/scripts/init.sh ${D}${libdir}/telegraf/scripts/
    #install -m 0644 ${S}/usr/lib/telegraf/scripts/telegraf.service ${D}${libdir}/telegraf/scripts/
    fi
    # /var - for /var/log/telegraf/telegraf.log
    install -d ${D}${localstatedir}/log/telegraf
}

# --> systemd service
# please note, that above we already copy files depeding on sysvinit/systemd
#REQUIRED_DISTRO_FEATURES= "systemd"
inherit systemd
SYSTEMD_AUTO_ENABLE = "enable"
# disable for manual testing
# e.g. on target:
# systemctl start telegraf.service
#SYSTEMD_AUTO_ENABLE = "disable"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "${PN}.service"
# <-- systemd service

# --> sysvinit scripts
# please note, that above we already copy files depeding on sysvinit/systemd
INITSCRIPT_PACKAGES = "${PN}"
INITSCRIPT_NAME_${PN} = "${PN}"
# script has a runlevel of: 99
# starts in initlevels:     2 3 4 5
# stops  in initlevels: 0 1         6
INITSCRIPT_PARAMS_${PN} = "start 99 2 3 4 5 . stop 99 0 1 6 ."
# <-- sysvinit scripts
