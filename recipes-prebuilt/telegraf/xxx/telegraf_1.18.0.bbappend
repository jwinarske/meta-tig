# telegraf version and machine specific changes:
FILESEXTRAPATHS_prepend := "${THISDIR}/${PV}/${MACHINE}:"

SRC_URI_append_imx6ul-phytec-segin = " \
         file://telegraf.conf \
"

SRC_URI_append_stm32mp157c-dk2 = " \
         file://telegraf.conf \
"

SRC_URI_append_einstein = " \
         file://telegraf.conf \
"

do_install_append () {
if [ -f ${WORKDIR}/telegraf.conf ]; then
   cp ${WORKDIR}/telegraf.conf ${D}${sysconfdir}/telegraf/
fi
}

### temporarily only for license experiments begin
# --> license detector - does not work - needs pkgs
# do_devshell[depends] += "github.com-google-go-license-detector-native:do_populate_sysroot"
# <-- license detector
### temporarily only for license experiments end




