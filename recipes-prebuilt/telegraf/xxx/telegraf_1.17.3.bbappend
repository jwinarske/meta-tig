# telegraf version and machine specific changes:
FILESEXTRAPATHS_prepend := "${THISDIR}/${PV}/${MACHINE}:"

SRC_URI_append_imx6ul-phytec-segin = " \
         file://telegraf.conf \
"

SRC_URI_append_stm32mp157c-dk2 = " \
         file://telegraf.conf \
"

do_install_append () {
if [ -f ${WORKDIR}/telegraf.conf ]; then
   cp ${WORKDIR}/telegraf.conf ${D}${sysconfdir}/telegraf/
fi
}
