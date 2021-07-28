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

SRC_URI_append_beagle-bone-black = " \
         file://telegraf.conf \
"

SRC_URI_append_phycore-stm32mp1-2 = " \
         file://telegraf.conf \
"

SRC_URI_append_imx6q-phytec-mira-rdk-nand = " \
         file://telegraf.conf \
"

do_install_append () {
if [ -f ${WORKDIR}/telegraf.conf ]; then
   cp ${WORKDIR}/telegraf.conf ${D}${sysconfdir}/telegraf/

   # overwrite config file with custom config file, if it exists
   if [ -f ${WORKDIR}/${cfg-file} ]; then
      install -m 0644 ${WORKDIR}/${cfg-file} ${D}${sysconfdir}/telegraf/telegraf.conf
   fi
fi
}

### temporarily only for license experiments begin
# --> license detector - does not work - needs pkgs
# do_devshell[depends] += "github.com-google-go-license-detector-native:do_populate_sysroot"
# <-- license detector
### temporarily only for license experiments end




