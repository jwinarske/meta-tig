# Copyright (C) 2020 Robert Berger <robert.berger@ReliableEmbeddedSystems.com>
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "A telegraf-prebuilt container image packaged to go on the rootfs and be loaded by docker"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

# --> ====== multiconfig magic =====

RESY_CONTAINER = "resy-container"

NATIVE_MACHINE ??= "imx6q-phytec-mira-rdk-nand-resy-virt"

CONTAINER_APP_SHORT="telegraf"
CONTAINER_APP="${CONTAINER_APP_SHORT}-prebuilt"
CONTAINER_SPECIFIC_PATH="${RESY_CONTAINER}/${CONTAINER_APP}"

CONTAINER_MACHINE="container-arm-v7"
CONTAINER_DISTRO="resy-container"
CONTAINER_IMAGE="app-container-image-${CONTAINER_APP}-oci"
CONTAINER_TMP="tmp-${CONTAINER_MACHINE}-${CONTAINER_DISTRO}"
CONTAINER_TAR_BZ2="app-container-image-${CONTAINER_APP}-oci-container-arm-v7.tar.bz2"
CONTAINER_TAR_BZ2_PATH="${CONTAINER_TMP}/deploy/images/${CONTAINER_MACHINE}"
CONTAINER_INSTALL_PATH="${RESY_CONTAINER}/prepopulated-container-images"

CONTAINER_DOCKER_COMPOSE="${RESY_CONTAINER}/docker-compose"
CONTAINER_SPECIFIC_PATH="${RESY_CONTAINER}/${CONTAINER_APP}"

# syntax:
#task_or_package[mcdepends] = "mc:from_multiconfig:to_multiconfig:recipe_name:task_on_which_to_depend"
do_fetch[mcdepends] = "mc:${NATIVE_MACHINE}:${CONTAINER_MACHINE}-${CONTAINER_DISTRO}:${CONTAINER_IMAGE}:do_image_complete"

# <-- ===== multiconfig magic =====

SRC_URI += " \
    file://${TOPDIR}/${CONTAINER_TAR_BZ2_PATH}/${CONTAINER_TAR_BZ2};unpack=0 \
    file://docker-compose.yml \
    file://telegraf.conf \
    file://wait-for-file.sh \
    file://create-inflxdb-name.sh \
    file://device-identity \
"
do_install() {
	# place container tarball in rootfs
        install -d ${D}/${rootdir}/${CONTAINER_INSTALL_PATH}
        install -m 0444 ${WORKDIR}/${TOPDIR}/${CONTAINER_TAR_BZ2_PATH}/${CONTAINER_TAR_BZ2} \
                        ${D}/${rootdir}/${CONTAINER_INSTALL_PATH}/${CONTAINER_TAR_BZ2}
	# telegraf specific stuff - so we can have e.g. telegraf.conf - volume mounted
        install -d ${D}/${rootdir}/${CONTAINER_SPECIFIC_PATH}/etc/telegraf
        install -m 0444 ${WORKDIR}/telegraf.conf ${D}/${rootdir}/${CONTAINER_SPECIFIC_PATH}/etc/telegraf/telegraf.conf
        # needed for docker-compose + telegraf init script
        install -d ${D}/${rootdir}/${CONTAINER_SPECIFIC_PATH}/telegraf-from-host
        install -m 0775 ${WORKDIR}/wait-for-file.sh ${D}/${rootdir}/${CONTAINER_SPECIFIC_PATH}/telegraf-from-host/wait-for-file.sh
	# special hacks to dynamically update telegraf.conf
        install -m 0775 ${WORKDIR}/create-inflxdb-name.sh ${D}/${rootdir}/${CONTAINER_SPECIFIC_PATH}/create-inflxdb-name.sh
	install -m 0775 ${WORKDIR}/device-identity ${D}/${rootdir}/${CONTAINER_SPECIFIC_PATH}/device-identity
        # for now only one docker-compose
        install -d ${D}/${rootdir}/${CONTAINER_DOCKER_COMPOSE}/${CONTAINER_APP}
        install -m 0444 ${WORKDIR}/docker-compose.yml \
           ${D}/${rootdir}/${CONTAINER_DOCKER_COMPOSE}/${CONTAINER_APP}/docker-compose.yml
}

FILES_${PN} += "/${CONTAINER_INSTALL_PATH}/${CONTAINER_TAR_BZ2} \
                /${CONTAINER_DOCKER_COMPOSE}/${CONTAINER_APP}/docker-compose.yml \
                /${CONTAINER_SPECIFIC_PATH}/etc/telegraf/telegraf.conf \
                /${CONTAINER_SPECIFIC_PATH}/telegraf-from-host/wait-for-file.sh \
		/${CONTAINER_SPECIFIC_PATH}/create-inflxdb-name.sh \
                /${CONTAINER_SPECIFIC_PATH}/device-identity \
"
# --> systemd service
REQUIRED_DISTRO_FEATURES= "systemd"
inherit systemd
SYSTEMD_AUTO_ENABLE = "enable"
# disable for manual testing
# e.g. on target:
# systemctl start docker-compose-telegraf-prebuilt
#SYSTEMD_AUTO_ENABLE = "disable"
SYSTEMD_SERVICE_${PN} = "docker-compose-telegraf-prebuilt.service"

SRC_URI += "file://docker-compose-telegraf-prebuilt.service"

FILES_${PN} += "${systemd_unitdir}/system/docker-compose-telegraf-prebuilt.service"

do_install_append () {
	install -d ${D}${systemd_unitdir}/system
	install -c -m 0644 ${WORKDIR}/docker-compose-telegraf-prebuilt.service ${D}${systemd_unitdir}/system
}
# <-- systemd service
