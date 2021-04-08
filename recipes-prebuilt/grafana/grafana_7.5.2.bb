require grafana.inc

SRC_URI_armv7a = "https://dl.grafana.com/oss/release/${BPN}-${PV}.linux-armv7.tar.gz \
                  file://LICENSE"
SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'armv7a', '7356fb76d1479a39365ec09484633beec42be04b4ffe7ce10bd0a08d120ff6c8', '', d)}"

SRC_URI_x86-64 = "https://dl.grafana.com/oss/release/${BPN}-${PV}.linux-amd64.tar.gz \
                  file://LICENSE"
SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'x86-64', 'c95b3030d38b5ff005b6fe394d79bc59c1c4cadc9882d8ef8bb5747517b00dd7', '', d)}"

LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=31f6db4579f7bbf48d02bff8c5c3a6eb"

LICENSE = "Apache-2.0"

#DEPENDS_x86-64 = "zlib fontconfig freetype"
