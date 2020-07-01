DESCRIPTION = "github.com/influxdata/influxdb"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://src/${GO_IMPORT}/LICENSE;md5=f39a8d10930fb37bd59adabb3b9d0bd6"

# DEPENDS don't seem to be needed in my poky build container
#DEPENDS += "gdm-native mercurial-native git-native"
#DEPENDS += "mercurial-native git-native"
GO_IMPORT = "github.com/influxdata/influxdb"

GOBUILDFLAGS_remove = "-buildmode=pie"
inherit go go-mod pythonnative systemd
BRANCH = "nobranch=1"
SRCREV = "781490de48220d7695a05c29e5a36f550a4568f5"

SRC_URI = "git://github.com/influxdata/influxdb;${BRANCH};protocol=https;destsuffix=${BPN}-${PV}/src/${GO_IMPORT} \
           file://influxdb.service \
           file://influxdb.conf \
"

export GO111MODULE="on"

do_configure_append() {
        sed -i -e "s%/usr/bin/python2.7.*%/usr/bin/env python2.7%g" ${S}/src/${GO_IMPORT}/build.py
}

do_install_append() {
        if [ "${@bb.utils.contains("DISTRO_FEATURES", "systemd", "yes", "no", d)}" = "yes" ]; then
                install -D -m 0644 ${WORKDIR}/influxdb.service ${D}${systemd_unitdir}/system/influxdb.service
        fi
        install -D -m 0644 ${WORKDIR}/influxdb.conf ${D}${sysconfdir}/influxdb/influxdb.conf
        rm -rf ${D}${libdir}/go/pkg/mod/cloud.google.com/go@v0.51.0/cmd/go-cloud-debug-agent/internal/debug/dwarf/testdata
        rm -rf ${D}${libdir}/go/pkg/mod/cloud.google.com/go@v0.51.0/cmd/go-cloud-debug-agent/internal/debug/elf/testdata
}

SYSTEMD_SERVICE_${PN} = "influxdb.service"

FILES_${PN} += "${GOBIN_FINAL}/*"

RDEPENDS_${PN}-staticdev += "\
                             perl \
                             "
RDEPENDS_${PN}-dev += "\
                       bash \
                       python \
                       "
