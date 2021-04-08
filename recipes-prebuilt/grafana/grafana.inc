DESCRIPTION = "The tool for beautiful monitoring and metric analytics & dashboards for Graphite, InfluxDB & Prometheus & More"

SRC_URI_append = " \
    file://grafana.service \
    file://grafana-server \
"

inherit systemd

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/bin/grafana-cli ${D}${bindir}/grafana-cli
    install -m 0755 ${S}/bin/grafana-server ${D}${bindir}/grafana-server

    install -d ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/grafana.service ${D}${systemd_system_unitdir}/grafana.service
    
    install -d ${D}${sysconfdir}/default
    install -m 0644 ${WORKDIR}/grafana-server ${D}${sysconfdir}/default/
    
    install -d ${D}${sysconfdir}/grafana
    install -m 0644 ${S}/conf/sample.ini ${D}${sysconfdir}/grafana/grafana.ini

    for d in dashboards datasources notifiers
    do
        install -d ${D}${sysconfdir}/grafana/provisioning/${d}
        install -m 0644 ${S}/conf/provisioning/${d}/sample.yaml ${D}${sysconfdir}/grafana/provisioning/${d}/sample.yaml
    done

    # install frontend
    install -d ${D}${datadir}/grafana

    cp -R --no-dereference --preserve=mode,links -v \
      ${S}/public \
      ${D}${datadir}/grafana/

    cp -R --no-dereference --preserve=mode,links -v \
      ${S}/conf \
      ${D}${datadir}/grafana/

    # grafana 6.5.2, !7.5.2
    if [ -d ${S}/tools ]; then
    cp -R --no-dereference --preserve=mode,links -v \
      ${S}/tools \
      ${D}${datadir}/grafana/
    fi

    # grafana 7.5.2, !6.5.2
    if [ -d ${S}/plugins-bundled ]; then
    cp -R --no-dereference --preserve=mode,links -v \
      ${S}/plugins-bundled \
      ${D}${datadir}/grafana/
    fi

    cp -R --no-dereference --preserve=mode,links -v \
      ${S}/LICENSE \
      ${D}${datadir}/grafana/

    cp -R --no-dereference --preserve=mode,links -v \
      ${S}/VERSION \
      ${D}${datadir}/grafana/

# this works for grafana 6.5.2:
#   cp -R --no-dereference --preserve=mode,links -v \
#    	${S}/public \
#	${S}/conf \
#	${S}/tools \
#	${S}/LICENSE \
#	${S}/VERSION \
#    	${D}${datadir}/grafana/
}

INSANE_SKIP_${PN} = "ldflags already-stripped build-deps"

SYSTEMD_SERVICE_${PN} = "\
    grafana.service \
"

SYSTEMD_AUTO_ENABLE_${PN} = "disable"

FILES_${PN} += "\
    ${systemd_unitdir} \
    ${sysconfdir}/grafana \
    ${sysconfdir}/default \
"
