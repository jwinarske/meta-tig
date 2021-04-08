#SRC_URI = "https://dl.grafana.com/oss/release/grafana-${PV}.linux-amd64.tar.gz;name=amd64"
#SRC_URI_armv6 = "https://dl.grafana.com/oss/release/grafana-${PV}.linux-armv6.tar.gz;name=armv6"
#SRC_URI_armv7a = "https://dl.grafana.com/oss/release/grafana-${PV}.linux-armv7.tar.gz;name=armv7"
#SRC_URI_armv7ve = "https://dl.grafana.com/oss/release/grafana-${PV}.linux-armv7.tar.gz;name=armv7"
#SRC_URI_armv8a = "https://dl.grafana.com/oss/release/grafana-${PV}.linux-arm64.tar.gz;name=arm64"

#SRC_URI[amd64.md5sum] = "bbb232ba9b7aaf1e7d226a49564d712b"
#SRC_URI[amd64.sha256sum] = "0a8bc55949aa920682b3bde99e9b1b87eef2c644bde8f8a48fa3ac746920d2ba"
#SRC_URI[armv7.md5sum] = "7378b2bc0bd9b6a20f2e3b6e146b6aeb"
#SRC_URI[armv7.sha256sum] = "c34ee5332d161ef20a63b1d281f782870bc96d85bfe97d0a7802da7f7cd6a6ec"
#SRC_URI[armv6.md5sum] = "c6e9f2cc83d93d94dd02bc53a7499dc3"
#SRC_URI[armv6.sha256sum] = "2496dfd7df31c3dfe0d978606f451760d1abf02bb9f582bb92fefa7720b53576"
#SRC_URI[arm64.md5sum] = "7378b2bc0bd9b6a20f2e3b6e146b6aeb"
#SRC_URI[arm64.sha256sum] = "c34ee5332d161ef20a63b1d281f782870bc96d85bfe97d0a7802da7f7cd6a6ec"

#LICENSE = "Apache-2.0"
#LIC_FILES_CHKSUM = "file://LICENSE;md5=31f6db4579f7bbf48d02bff8c5c3a6eb"

require grafana.inc

SRC_URI_armv7a = "https://dl.grafana.com/oss/releass/${BPN}-${PV}.linux-armv7.tar.gz \
                  file://LICENSE"
SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'armv7a', '42d9ef7ce061b10257c37027fdc210fc760ae8de60f1424074a854d1f9363b3d', '', d)}"

SRC_URI_x86-64 = "https://dl.grafana.com/oss/release/${BPN}-${PV}.linux-amd64.tar.gz \
                  file://LICENSE"
SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'x86-64', '0a8bc55949aa920682b3bde99e9b1b87eef2c644bde8f8a48fa3ac746920d2ba', '', d)}"

LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=31f6db4579f7bbf48d02bff8c5c3a6eb"

LICENSE = "Apache-2.0"

DEPENDS_x86-64 = "zlib fontconfig freetype"
