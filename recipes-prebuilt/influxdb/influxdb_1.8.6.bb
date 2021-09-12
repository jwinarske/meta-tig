require influxdb.inc

# --> armhf
SRC_URI:armv7a = "https://dl.influxdata.com/influxdb/releases/influxdb-${PV}_linux_armhf.tar.gz;\
sha256sum=bf77801cc85a9e9f1710f38dc1263b5202e4d8f83c56a53501b033d6ade384a3 \
file://LICENSE"

SRC_URI_einstein = "https://dl.influxdata.com/influxdb/releases/influxdb-${PV}_linux_armhf.tar.gz;\
sha256sum=bf77801cc85a9e9f1710f38dc1263b5202e4d8f83c56a53501b033d6ade384a3 \
file://LICENSE"
# <-- armhf

# --> amd64
SRC_URI:x86-64 = "https://dl.influxdata.com/influxdb/releases/influxdb-${PV}-static_linux_amd64.tar.gz;\
sha256sum=c92ddb7fbe85575f42834b4a28828d6d6bf549b764d7fd90a711ba2e8d84122c \
file://LICENSE"
# <-- amd64

LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=f39a8d10930fb37bd59adabb3b9d0bd6"

# The open source license for InfluxDB is available in the GitHub repository.
# https://github.com/influxdata/influxdb/blob/1.8/LICENSE
# I copied LICENSE from there to my meta data
LICENSE = "MIT"

S = "${WORKDIR}/${PN}-${PV}-1"
