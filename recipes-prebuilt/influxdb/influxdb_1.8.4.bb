require influxdb.inc

SRC_URI_armv7a = "https://dl.influxdata.com/influxdb/releases/influxdb-${PV}_linux_armhf.tar.gz \
                  file://LICENSE"
SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'armv7a', 'cce5096bf5812d40e92353bc077359155ddffb9f3bab33ccc1ed82699cf75e73', '', d)}"

SRC_URI_x86-64 = "https://dl.influxdata.com/influxdb/releases/influxdb-${PV}-static_linux_amd64.tar.gz \
                  file://LICENSE"
SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'x86-64', '710d3346ad2e15d379388e43f187097e3733ec68d38a1dfb4b3b1c5fb3de56c1', '', d)}"

LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=f39a8d10930fb37bd59adabb3b9d0bd6"

# The open source license for InfluxDB is available in the GitHub repository.
# https://github.com/influxdata/influxdb/blob/1.8/LICENSE
# I copied LICENSE from there to my meta data
LICENSE = "MIT"

S = "${WORKDIR}/${PN}-${PV}-1"
