require telegraf.inc

# --> armhf
SRC_URI_armv7a = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_linux_armhf.tar.gz;\
sha256sum=e5607606015772b88df6de098a6a061b68b47cb8d56986ce688a39af53640e47 \
                    file://LICENSE"

SRC_URI_einstein = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_linux_armhf.tar.gz;\
sha256sum=e5607606015772b88df6de098a6a061b68b47cb8d56986ce688a39af53640e47 \
                    file://LICENSE"

SRC_URI_phycore-stm32mp1-2 = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_linux_armhf.tar.gz;\
sha256sum=e5607606015772b88df6de098a6a061b68b47cb8d56986ce688a39af53640e47 \
                    file://LICENSE"
# <-- armhf

# --> amd64
SRC_URI_x86-64 = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_static_linux_amd64.tar.gz;\
sha256sum=db7dd0bf234290dcebe267aa8bf9e5c941172f443211834282c370741b908a4d \
                    file://LICENSE"
# <-- amd64

LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=c5d3aeddd4f7a4c4993bbdc4a41aec44"

LICENSE = "MIT"

S = "${WORKDIR}/${PN}-${PV}"
