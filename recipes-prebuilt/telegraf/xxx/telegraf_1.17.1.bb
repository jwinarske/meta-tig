require telegraf.inc

SRC_URI_armv7a = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_linux_armhf.tar.gz \
                  file://LICENSE"
SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'armv7a', '9839a1af9f740fc52cdb749b848872aa2043a6d9daa5642dea19e1e302bbe2c7', '', d)}"

SRC_URI_x86-64 = "https://dl.influxdata.com/telegraf/releases/${BPN}-${PV}_static_linux_amd64.tar.gz \
                  file://LICENSE"
SRC_URI[sha256sum] = "${@bb.utils.contains('MACHINEOVERRIDES', 'x86-64', 'bd9d18836f5acda701325ad523019fc0e82435bc5b1a8ee93ebf3b9ac8109510', '', d)}"

LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=c5d3aeddd4f7a4c4993bbdc4a41aec44"

# The open source license for Telegraf is available in the GitHub repository.
# https://github.com/influxdata/telegraf/blob/v1.15.1/LICENSE
# I copied LICENSE from there to my meta data
LICENSE = "MIT"

S = "${WORKDIR}/${PN}-${PV}"

# telegraf-1.15.1_static_linux_amd64.tar.gz:
# telegraf-1.15.1/
# ├── etc
# │   ├── logrotate.d
# │   │   └── telegraf
# │   └── telegraf
# │       ├── telegraf.conf
# │       └── telegraf.d
# ├── usr
# │   ├── bin
# │   │   └── telegraf
# │   └── lib
# │       └── telegraf
# │           └── scripts
# │               ├── init.sh
# │               └── telegraf.service
# └── var
#     └── log
#         └── telegraf


