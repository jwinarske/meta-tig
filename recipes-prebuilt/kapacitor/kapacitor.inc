DESCRIPTION = "Kapacitor"
SUMMARY = "Kapacitor is a real-time streaming data processing engine."
HOMEPAGE = "https://www.influxdata.com/time-series-platform/kapacitor/"

###############################################################################
# Note:
# Platform support
# Kapacitor 1.6+ no longer supports 32-bit operating systems. 
# If you are using a 32-bit operating system, continue using Kapacitor 1.5.x.
###############################################################################

INSANE_SKIP:${PN}:append = " already-stripped"

# --> user/group
inherit useradd

# create kapacitor group
# --system                  create a system account
GROUPADD_PARAM:${PN} = "--system kapacitor"

# create kapacitor user
# --system                  create a system account
# --gid GROUP               name or ID of the primary group of the new
#                           account
USERADD_PARAM:${PN} += "kapacitor \
--system \
--no-create-home \
--home-dir /var/run/${BPN} \
--shell /bin/false \
--gid kapacitor \
"

# USERADD_PACKAGES specifies the output packages
# which include custom users/groups.
USERADD_PACKAGES = "${PN}"
# <-- user/group

# for the time being only systemd is supported
SRC_URI:append = " ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'file://volatiles.kapacitor.conf', 'file://volatiles.99_kapacitor', d)}"

FILES:${PN} += "${datadir}/*"

do_install() {
    #${sysconfdir} = /etc
    #${bindir} = /usr/bin
    #${libdir} = /usr/lib
    #${datadir} = /usr/share
    #${localstatedir} = /var

    # /etc
    install -d ${D}${sysconfdir}/kapacitor
    install -d ${D}${sysconfdir}/logrotate.d

    # ! x86-64 static:
    if [ -f ${S}/etc/kapacitor/kapacitor.conf ]; then
    install -m 0644 ${S}/etc/kapacitor/kapacitor.conf ${D}${sysconfdir}/kapacitor/
    fi
 
    # x86-64 static:
    if [ -f ${S}/kapacitor.conf ]; then
    install -m 0644 ${S}/kapacitor.conf ${D}${sysconfdir}/kapacitor/
    fi

    # ! x86-64 static:
    if [ -f ${S}/etc/logrotate.d/kapacitor ]; then
    install -m 0644 ${S}/etc/logrotate.d/kapacitor ${D}${sysconfdir}/logrotate.d/
    fi

    # /usr/bin
    install -d ${D}${bindir}

    # ! x86-64 static:
    if [ -d ${S}/usr/bin ]; then
    install -m 0755 ${S}/usr/bin/kapacitor ${D}${bindir}/
    install -m 0755 ${S}/usr/bin/kapacitord ${D}${bindir}/
    install -m 0755 ${S}/usr/bin/tickfmt ${D}${bindir}/
    else
    install -m 0755 ${S}/kapacitor ${D}${bindir}/
    install -m 0755 ${S}/kapacitord ${D}${bindir}/
    install -m 0755 ${S}/tickfmt ${D}${bindir}/
    fi

    # /usr/lib
    # --> systemd
    # Only install the script if 'systemd' is in DISTRO_FEATURES
    if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd','true','false',d)}; then
      # ! x86-64 static:
      if [ -f ${S}/usr/lib/kapacitor/scripts/kapacitor.service ]; then
         install -d ${D}${systemd_unitdir}/system
         # we have created a kapacitor user and group
         #sed -i 's/User=kapacitor/User=root/g' ${S}/usr/lib/kapacitor/scripts/kapacitor.service
         #sed -i 's/Group=kapacitor/Group=root/g' ${S}/usr/lib/kapacitor/scripts/kapacitor.service
         install -m 0644 ${S}/usr/lib/kapacitor/scripts/kapacitor.service ${D}${systemd_unitdir}/system
         #install -m 0755 ${S}/usr/lib/kapacitor/scripts/init.sh ${D}${libdir}/kapacitor/scripts/
      fi
    fi 
    # <-- systemd
    
    # --> sysvinit
       # Only install the script if 'sysvinit' is in DISTRO_FEATURES
       # systemd would be the other choice
       if ${@bb.utils.contains('DISTRO_FEATURES','sysvinit','true','false',d)}; then
           install -d ${D}${sysconfdir}/init.d/
           # --> /bin/sh instead of /bin/bash
           sed -i 's,#!/bin/bash,#! /bin/sh,g' ${S}/usr/lib/kapacitor/scripts/init.sh
           # <-- /bin/sh instead of /bin/bash

          # --> let's try a hack to fix permissions on /var/log/kapacitor/kapacitor.log
          sed -i '/log_success_msg "Starting the process" "$NAME"/a \ \n \t# --> try to fix permissions on /var/log/kapacitor/kapacitor.log\n\ \tif [[ ! -f /var/log/kapacitor/kapacitor.log ]]; then\n\ \t   touch /var/log/kapacitor/kapacitor.log\n\ \t   chown kapacitor:kapacitor /var/log/kapacitor/kapacitor.log\n\ \tfi\n\ \t# <-- try to fix permissions on /var/log/kapacitor/kapacitor.log\n' ${S}/usr/lib/kapacitor/scripts/init.sh
          # <-- let's try a hack to fix permissions on /var/log/kapacitor/kapacitor.log

           install -D -m 0755 ${S}/usr/lib/kapacitor/scripts/init.sh ${D}${sysconfdir}/init.d/kapacitor

           # volatile log files
           install -d ${D}${sysconfdir}/default/volatiles
           install -m 644 ${WORKDIR}/volatiles.99_kapacitor ${D}${sysconfdir}/default/volatiles/99_kapacitor
       fi
     # <-- sysvinit
    

    # ! x86-64 static:
    if [ -f ${S}/usr/share/bash-completion/completions/kapacitor ]; then
    # /usr/share
    install -d ${D}${datadir}/bash-completion/completions

    install -m 0644 ${S}/usr/share/bash-completion/completions/kapacitor ${D}${datadir}/bash-completion/completions/
    fi

    # /var/lib
    #install -d ${D}${localstatedir}/lib/kapacitor
    #install -d ${D}${localstatedir}/log/kapacitor

    # we need /var/lib/kapacitor u:kapacitor/g:kapacitor
    mkdir -p ${D}${localstatedir}/lib/kapacitor
    chown kapacitor:kapacitor ${D}${localstatedir}/lib/kapacitor


}

#inherit ${@bb.utils.contains('PACKAGECONFIG', 'scripting', 'python3native', '', d)}


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
# systemctl start kapacitor.service
#SYSTEMD_AUTO_ENABLE = "disable"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "${PN}.service"
# <-- systemd service

