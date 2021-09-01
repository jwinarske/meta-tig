require grafana.inc

# --> armv7
SRC_URI:armv7a = "https://dl.grafana.com/oss/release/${BPN}-${PV}.linux-armv7.tar.gz;\
sha256sum=5e801cef79c6e92ef51d0fc7fb7330b342fc082bcf089bea68060a38206d3253 \
file://LICENSE"

SRC_URI_einstein = "https://dl.grafana.com/oss/release/${BPN}-${PV}.linux-armv7.tar.gz;\
sha256sum=5e801cef79c6e92ef51d0fc7fb7330b342fc082bcf089bea68060a38206d3253 \
file://LICENSE"

SRC_URI_imx6q-phytec-mira-rdk-nand = "https://dl.grafana.com/oss/release/${BPN}-${PV}.linux-armv7.tar.gz;\
sha256sum=5e801cef79c6e92ef51d0fc7fb7330b342fc082bcf089bea68060a38206d3253 \
file://LICENSE"

# <-- armv7

# --> amd64
SRC_URI:x86-64 = "https://dl.grafana.com/oss/release/${BPN}-${PV}.linux-amd64.tar.gz;\
sha256sum=194567c3690ac557f3b6f7ec34cc112f13d3a916336f15f3a07d86926641d891 \
file://LICENSE"
# <-- amd64

LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=31f6db4579f7bbf48d02bff8c5c3a6eb"

LICENSE = "Apache-2.0"

#DEPENDS:x86-64 = "zlib fontconfig freetype"
