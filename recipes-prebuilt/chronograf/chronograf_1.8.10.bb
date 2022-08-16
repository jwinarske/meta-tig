require chronograf.inc

SRC_URI:armv7a = "https://dl.influxdata.com/chronograf/releases/${BPN}-${PV}_linux_armhf.tar.gz;\
sha256sum=ec03c0295ea497d5c4ad9a2a8e474e98a099431db3499f8f7fa160743f520dcb \
file://LICENSE"

SRC_URI:x86-64 = "https://dl.influxdata.com/chronograf/releases/${BPN}-${PV}_linux_amd64.tar.gz;\
sha256sum=3ce7f317d4f131c21977d8d4c44170c8e9bb5180605fbe8d11bc7b0bc4bd18b1 \
file://LICENSE"

LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=2156ff37b015d630167ea0b391b090aa"

S = "${WORKDIR}/${PN}-${PV}-1"

LICENSE = "AGPL-3.0-or-later"
