DESCRIPTION = "Kapacitor"
SUMMARY = "Kapacitor is a real-time streaming data processing engine."
HOMEPAGE = "https://www.influxdata.com/time-series-platform/kapacitor/"

INSANE_SKIP:${PN}:append = " already-stripped"

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
    # ! x86-64 static:
    if [ -f ${S}/usr/lib/kapacitor/scripts/kapacitor.service ]; then
    install -d ${D}${systemd_unitdir}/system
    sed -i 's/User=kapacitor/User=root/g' ${S}/usr/lib/kapacitor/scripts/kapacitor.service
    sed -i 's/Group=kapacitor/Group=root/g' ${S}/usr/lib/kapacitor/scripts/kapacitor.service
    install -m 0644 ${S}/usr/lib/kapacitor/scripts/kapacitor.service ${D}${systemd_unitdir}/system
    #install -m 0755 ${S}/usr/lib/kapacitor/scripts/init.sh ${D}${libdir}/kapacitor/scripts/
    #install -m 0644 ${S}/usr/lib/kapacitor/scripts/kapacitor.service ${D}${libdir}/kapacitor/scripts/
    fi

    # ! x86-64 static:
    if [ -f ${S}/usr/share/bash-completion/completions/kapacitor ]; then
    # /usr/share
    install -d ${D}${datadir}/bash-completion/completions

    install -m 0644 ${S}/usr/share/bash-completion/completions/kapacitor ${D}${datadir}/bash-completion/completions/
    fi

    # /var/lib
    install -d ${D}${localstatedir}/lib/kapacitor
    install -d ${D}${localstatedir}/log/kapacitor

}

#inherit ${@bb.utils.contains('PACKAGECONFIG', 'scripting', 'python3native', '', d)}

inherit systemd
SYSTEMD_SERVICE:${PN} = "kapacitor.service"
