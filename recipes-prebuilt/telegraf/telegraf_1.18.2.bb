require telegraf.inc

#SRC_URI_armv7a = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_linux_armhf.tar.gz \
#                  file://LICENSE"
#SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'armv7a', '42d9ef7ce061b10257c37027fdc210fc760ae8de60f1424074a854d1f9363b3d', '', d)}"

#SRC_URI_x86-64 = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_static_linux_amd64.tar.gz \
#                  file://LICENSE"
#SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'x86-64', '64042215eea66fd03bc5ba79b98304de0402c35b178c39847c5ea50931961178', '', d)}"


# --> armhf
SRC_URI_armv7a = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_linux_armhf.tar.gz;\
md5sum=e86442a9174b599aaf99fff22cc224b6;\
sha256sum=e5607606015772b88df6de098a6a061b68b47cb8d56986ce688a39af53640e47 \
                    file://LICENSE"

SRC_URI_einstein = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_linux_armhf.tar.gz;\
md5sum=e86442a9174b599aaf99fff22cc224b6;\
sha256sum=e5607606015772b88df6de098a6a061b68b47cb8d56986ce688a39af53640e47 \
                    file://LICENSE"
# <-- armhf

# --> amd64
SRC_URI_x86-64 = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_static_linux_amd64.tar.gz;\
md5sum=7d499d8516239b42fae0b4b47f831597;\
sha256sum=db7dd0bf234290dcebe267aa8bf9e5c941172f443211834282c370741b908a4d \
                    file://LICENSE"
# <-- amd64

LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=c5d3aeddd4f7a4c4993bbdc4a41aec44"

LICENSE = "MIT"

S = "${WORKDIR}/${PN}-${PV}"
