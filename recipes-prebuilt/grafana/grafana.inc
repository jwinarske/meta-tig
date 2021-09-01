DESCRIPTION = "The tool for beautiful monitoring and metric analytics & dashboards for Graphite, InfluxDB & Prometheus & More"

SRC_URI:append = " ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'file://grafana.service', 'file://init.d-grafana-server', d)}"

SRC_URI:append = " file://grafana-server"

# --> user/group
#
# !!! it looks like we need to run grafana as root !!!
#
#inherit useradd

# create grafana group
# --system                  create a system account
#GROUPADD_PARAM:${PN} = "--system grafana"

# create influxdb user
# --system                  create a system account
# --gid GROUP               name or ID of the primary group of the new
#                           account
#USERADD_PARAM:${PN} += "grafana \
#--system \
#--no-create-home \
#--home-dir /var/run/${BPN} \
#--shell /bin/false \
#--gid grafana \
#"
##########################################################
# e.g.
#
# id grafana
# uid=996(grafana) gid=995(grafana) groups=995(grafana)
#
# cat /etc/group | grep grafana
# grafana:x:995:
#
# cat /etc/passwd | grep grafana
# grafana:x:996:995::/var/run/grafana:/bin/false
#
# cat /etc/shadow | grep grafana
# grafana:!:18810:0:99999:7:::
##########################################################

# USERADD_PACKAGES specifies the output packages
# which include custom users/groups.
#USERADD_PACKAGES = "${PN}"
# <-- user/group

inherit update-rc.d systemd

do_configure[noexec] = "1"
do_compile[noexec] = "1"

do_install () {
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

           # --> if we have DISTRO_FEATURE tig we want to control the startup sequence
           # 1) influxdb
           # 2) telegraf
           # 3) grafana
           if ${@bb.utils.contains('DISTRO_FEATURES', 'tig','true','false',d)}; then
              # --> various modifications to grafana.service in order to make sure it's 3)
              #    --> replace
              #        this line: Wants=network-online.target
              #        with this: #Wants=network-online.target
              sed -i 's/Wants=network-online.target/#Wants=network-online.target/' ${D}${systemd_unitdir}/system/grafana.service
              #    <-- replace
              #    --> replace
              #        this line: After=network-online.target
              #        with this: After=network-online.target telegraf.service
              sed -i 's/After=network-online.target/After=network-online.target telegraf.service/' ${D}${systemd_unitdir}/system/grafana.service
              #    <-- replace

              #    --> add
              #        after this line: WantedBy=multi-user.target
              #        this line:       Wants=network-online.target telegraf.service
              sed -i '/^WantedBy=multi-user.target/a Wants=network-online.target telegraf.service' ${D}${systemd_unitdir}/system/grafana.service
              #    <-- add
              # <-- various modifications to grafana.service in order to make sure it's 3)
           fi
           # <-- if we have DISTRO_FEATURE tig we want to control the startup sequence

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


# the problem is not stripping, but prelink
#INHIBIT_PACKAGE_STRIP = "1"
#INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
# explicitly tell do_package not to strip those files
#INHIBIT_PACKAGE_STRIP_FILES = "${PKGD}${bindir}/grafana-cli \
#${PKGD}${bindir}/grafana-server \
#"
#INSANE_SKIP:${PN} = "already-stripped"

#INSANE_SKIP:${PN} = "ldflags already-stripped build-deps"

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
INITSCRIPT_NAME:${PN} = "grafana-server"
# script has a runlevel of: 99
# starts in initlevels:     2 3 4 5
# stops  in initlevels: 0 1         6
INITSCRIPT_PARAMS:${PN} = "start 95 2 3 4 5 . stop 95 0 1 6 ."
# <-- sysvinit scripts

#SYSTEMD_SERVICE:${PN} = "\
#    grafana.service \
#"
#
#SYSTEMD_AUTO_ENABLE:${PN} = "disable"

FILES:${PN} += "\
    ${sysconfdir}/grafana \
    ${sysconfdir}/default \
"

# files we only need with systemd
FILES:${PN} += " ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', '${systemd_unitdir}', '', d)}"
