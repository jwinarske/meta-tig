DESCRIPTION = "The tool for beautiful monitoring and metric analytics & dashboards for Graphite, InfluxDB & Prometheus & More"

SRC_URI_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'file://grafana.service', 'file://init.d-grafana-server', d)}"

SRC_URI_append = " file://grafana-server"

# --> user/group
inherit useradd

# create grafana group
# --system                  create a system account
GROUPADD_PARAM_${PN} = "--system grafana"

# create influxdb user
# --system                  create a system account
# --gid GROUP               name or ID of the primary group of the new
#                           account
USERADD_PARAM_${PN} += "grafana \
--system \
--no-create-home \
--home-dir /var/run/${BPN} \
--shell /bin/false \
--gid grafana \
"
##########################################################
# e.g.
#
# id grafana
# uid=997(influxdb) gid=996(influxdb) groups=996(influxdb)
#
# cat /etc/group | grep grafana
# influxdb:x:996:
#
# cat /etc/passwd | grep grafana
# influxdb:x:997:996::/var/run/influxdb:/bin/false
#
# cat /etc/shadow | grep grafana
# influxdb:!:18809:0:99999:7:::
##########################################################

# USERADD_PACKAGES specifies the output packages
# which include custom users/groups.
USERADD_PACKAGES = "${PN}"
# <-- user/group



inherit update-rc.d systemd

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/bin/grafana-cli ${D}${bindir}/grafana-cli
    install -m 0755 ${S}/bin/grafana-server ${D}${bindir}/grafana-server

    # Only install the script if 'sysvinit' is in DISTRO_FEATURES
    # systemd would be the other choice
    if ${@bb.utils.contains('DISTRO_FEATURES','sysvinit','true','false',d)}; then
        install -d ${D}${sysconfdir}/init.d/
        install -D -m 0755 ${WORKDIR}/init.d-grafana-server ${D}${sysconfdir}/init.d/grafana-server 
    fi # sysvinit

    # Only install the script if 'systemd' is in DISTRO_FEATURES
    # systemd
    if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd','true','false',d)}; then
        install -d ${D}${systemd_unitdir}/system
        install -m 0644 ${WORKDIR}/grafana.service ${D}${systemd_system_unitdir}/grafana.service
    fi # systemd
    
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
SYSTEMD_SERVICE_${PN} = "${PN}.service"
# <-- systemd service

# --> sysvinit scripts
# please note, that above
#   we already copy files depeding on sysvinit/systemd
#   we already inherited update-rc.d
INITSCRIPT_PACKAGES = "${PN}"
INITSCRIPT_NAME_${PN} = "grafana-server"
# script has a runlevel of: 99
# starts in initlevels:     2 3 4 5
# stops  in initlevels: 0 1         6
INITSCRIPT_PARAMS_${PN} = "start 99 2 3 4 5 . stop 99 0 1 6 ."
# <-- sysvinit scripts

#SYSTEMD_SERVICE_${PN} = "\
#    grafana.service \
#"
#
#SYSTEMD_AUTO_ENABLE_${PN} = "disable"

FILES_${PN} += "\
    ${sysconfdir}/grafana \
    ${sysconfdir}/default \
"

# files we only need with systemd
FILES_${PN} += " ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', '${systemd_unitdir}', '', d)}"
