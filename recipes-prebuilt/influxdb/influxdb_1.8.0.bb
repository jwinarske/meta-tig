require influxdb.inc

SRC_URI_armv7a = "https://dl.influxdata.com/influxdb/releases/influxdb-${PN}_linux_armhf.tar.gz \
                  file://LICENSE"
SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'armv7a', 'fdac157f02e8231e1925d8e9bc325c88b7ba55ebab2340c549ef10640dbd0cba', '', d)}"

SRC_URI_x86-64 = "https://dl.influxdata.com/influxdb/releases/influxdb-${PN}-static_linux_amd64.tar.gz \
                  file://LICENSE"
SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'x86-64', 'aedc5083ae2e61ef374dbde5044ec2a5b27300e73eb92ccd135e6ff9844617e2', '', d)}"

LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=f39a8d10930fb37bd59adabb3b9d0bd6"

# The open source license for InfluxDB is available in the GitHub repository.
# https://github.com/influxdata/influxdb/blob/1.8/LICENSE
# I copied LICENSE from there to my meta data
LICENSE = "MIT"

S = "${WORKDIR}/${PN}-${PV}-1"
