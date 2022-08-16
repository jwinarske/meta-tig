require kapacitor.inc

SRC_URI:armv7a = "https://dl.influxdata.com/kapacitor/releases/${BPN}-${PV}_linux_armhf.tar.gz;\
sha256sum=94dbb589527daafe8239d1c9bdf9a134e05aa44a235e19199f395828d0377a2e \
file://LICENSE"

SRC_URI:x86-64 = "https://dl.influxdata.com/kapacitor/releases/${BPN}-${PV}-static_linux_amd64.tar.gz;\
sha256sum=ac50f6d4c75f15516fde0b36fbf547a09d6a3443af0c9a5fd9b6fb52322e59e6 \
file://LICENSE"

LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=96cd9a86f733dbfec4107613b9b27c71"

S = "${WORKDIR}/${PN}-${PV}-1"

LICENSE = "MIT"
