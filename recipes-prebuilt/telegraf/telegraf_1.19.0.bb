require telegraf.inc

# --> armhf
SRC_URI_armv7a = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_linux_armhf.tar.gz;\
sha256sum=c38f1975a86d0771c785aa455c086f2c0b4791d837ee6dc97330404d01aded99 \
                    file://LICENSE"

SRC_URI_einstein = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_linux_armhf.tar.gz;\
sha256sum=c38f1975a86d0771c785aa455c086f2c0b4791d837ee6dc97330404d01aded99 \
                    file://LICENSE"

SRC_URI_phycore-stm32mp1-2 = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_linux_armhf.tar.gz;\
sha256sum=c38f1975a86d0771c785aa455c086f2c0b4791d837ee6dc97330404d01aded99 \
                    file://LICENSE"
# <-- armhf

# --> amd64
SRC_URI_x86-64 = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_static_linux_amd64.tar.gz;\
sha256sum=f916eddac39897cc45b0186ade8c25b2a024fa29324ed435ebc31204a8c1a807 \
                    file://LICENSE"
# <-- amd64

LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=c5d3aeddd4f7a4c4993bbdc4a41aec44"

LICENSE = "MIT"

S = "${WORKDIR}/${PN}-${PV}"
