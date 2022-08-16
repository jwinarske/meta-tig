DESCRIPTION = "github.com/influxdata/influxdb"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://src/${GO_IMPORT}/LICENSE;md5=f39a8d10930fb37bd59adabb3b9d0bd6"

GO_IMPORT = "github.com/influxdata/influxdb"

GOBUILDFLAGS:remove = "-buildmode=pie"
inherit go go-mod python3native systemd
BRANCH = "nobranch=1"

SRCREV = "12892eb5f8e8f5cd94197d914a040128c359ad41"

#SRC_URI = "git://github.com/influxdata/influxdb;${BRANCH};protocol=https;tag=v${PV};destsuffix=${BPN}-${PV}/src/${GO_IMPORT}

SRC_URI = "git://github.com/influxdata/influxdb;${BRANCH};protocol=https;destsuffix=${BPN}-${PV}/src/${GO_IMPORT} \
           file://influxdb.service \
           file://influxdb.conf \
"
# this patch is needed for 1.8.10
SRC_URI += "file://bash-instead-of-sh.patch \
"

export GO111MODULE="on"

do_configure:append() {
        sed -i -e "s%/usr/bin/python2.7.*%/usr/bin/env python2.7%g" ${S}/src/${GO_IMPORT}/build.py
}

do_install:append() {
        if [ "${@bb.utils.contains("DISTRO_FEATURES", "systemd", "yes", "no", d)}" = "yes" ]; then
                install -D -m 0644 ${WORKDIR}/influxdb.service ${D}${systemd_unitdir}/system/influxdb.service
        fi
        install -D -m 0644 ${WORKDIR}/influxdb.conf ${D}${sysconfdir}/influxdb/influxdb.conf
        rm -rf ${D}${libdir}/go/pkg/mod/cloud.google.com/go@v0.51.0/cmd/go-cloud-debug-agent/internal/debug/dwarf/testdata
        rm -rf ${D}${libdir}/go/pkg/mod/cloud.google.com/go@v0.51.0/cmd/go-cloud-debug-agent/internal/debug/elf/testdata
}

SYSTEMD_SERVICE:${PN} = "influxdb.service"

FILES:${PN} += "${GOBIN_FINAL}/*"

RDEPENDS:${PN}-staticdev += "\
                             perl \
                             "
RDEPENDS:${PN}-dev += "bash"

#RDEPENDS:${PN}-dev += "\
#                       bash \
#                       python3 \
#                       "
