DESCRIPTION = "Telegraf"
SUMMARY = "Telegraf is the open source server agent to help you collect metrics from your stacks, sensors and systems."
HOMEPAGE = "https://www.influxdata.com/time-series-platform/telegraf/"

INSANE_SKIP_${PN}_append = "already-stripped"

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
    # x86-64 does not
    install -m 0755 ${S}/telegraf ${D}${bindir}/
    fi

    # /usr/lib
    if [ -d ${S}/usr/lib ]; then
    # only for armv7
    install -d ${D}${systemd_unitdir}/system
    sed -i 's/User=telegraf/User=root/g' ${S}/usr/lib/telegraf/scripts/telegraf.service
    install -m 0644 ${S}/usr/lib/telegraf/scripts/telegraf.service ${D}${systemd_unitdir}/system

    #install -m 0644 ${S}/usr/lib/telegraf/scripts/init.sh ${D}${libdir}/telegraf/scripts/
    #install -m 0644 ${S}/usr/lib/telegraf/scripts/telegraf.service ${D}${libdir}/telegraf/scripts/
    fi
    # /var
    install -d ${D}${localstatedir}/log/telegraf
    
}

inherit systemd
SYSTEMD_SERVICE_${PN} = "telegraf.service"

# telegraf-1.15.1_static_linux_amd64.tar.gz: finally !!!!
# telegraf-1.15.1/
# ├── etc
# │   ├── logrotate.d
# │   │   └── telegraf
# │   └── telegraf
# │       ├── telegraf.conf
# │       └── telegraf.d
# ├── usr
# │   ├── bin
# │   │   └── telegraf
# │   └── lib
# │       └── telegraf
# │           └── scripts
# │               ├── init.sh
# │               └── telegraf.service
# └── var
#     └── log
#         └── telegraf


# older x86-64 static:
# └── telegraf
#     ├── telegraf
#     └── telegraf.conf

# armv7:
# .
# └── telegraf
#     ├── etc
#     │   ├── logrotate.d
#     │   │   └── telegraf
#     │   └── telegraf
#     │       ├── telegraf.conf
#     │       └── telegraf.d
#     ├── usr
#     │   ├── bin
#     │   │   └── telegraf
#     │   └── lib
#     │       └── telegraf
#     │           └── scripts
#     │               ├── init.sh
#     │               └── telegraf.service
#     └── var
#         └── log
#             └── telegraf

