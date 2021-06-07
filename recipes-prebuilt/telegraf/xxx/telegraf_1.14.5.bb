require telegraf.inc

#PR = "r1"

SRC_URI_armv7a = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_linux_armhf.tar.gz \
                  file://LICENSE"
SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'armv7a', 'cf1333eebb6b4c330f59b7e2a251e7b00e891eed7670c794a142a70c1712760f', '', d)}"

SRC_URI_x86-64 = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}-static_linux_amd64.tar.gz \
                  file://LICENSE"
SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'x86-64', 'aedc5083ae2e61ef374dbde5044ec2a5b27300e73eb92ccd135e6ff9844617e2', '', d)}"

LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=c5d3aeddd4f7a4c4993bbdc4a41aec44"

# The open source license for Telegraf is available in the GitHub repository.
# https://github.com/influxdata/telegraf/blob/v1.14.5/LICENSE
# I copied LICENSE from there to my meta data
LICENSE = "MIT"

S = "${WORKDIR}/telegraf"
