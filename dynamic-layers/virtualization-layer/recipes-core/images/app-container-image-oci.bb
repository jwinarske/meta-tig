# Copyright (C) 2020 Robert Berger <robert.berger@ReliableEmbeddedSystems.com>
# Released under the MIT license (see COPYING.MIT for the terms)

require recipes-core/images/common-img.inc

SUMMARY = "A minimal container image"
LICENSE = "MIT"
LIC_FILES_CHECKSUM = "file://${COREBASE}/meta/CONFIG.MIT;md5=1234"

IMAGE_FSTYPES = "container oci"

inherit image
inherit image-oci

# rber added this:
# IMAGE_TYPEDEP_container += "ext4"

IMAGE_FEATURES = ""
IMAGE_LINGUAS = ""
NO_RECOMMENDATIONS = "1"

# Allow build with or without a specific kernel
IMAGE_CONTAINER_NO_DUMMY = "0"
# # 0 -> no kernel
# # 1 -> kernel

IMAGE_INSTALL = "\
	base-files \
	base-passwd \
	netbase \
"

# Workaround /var/volatile for now
ROOTFS_POSTPROCESS_COMMAND += "rootfs_fixup_var_volatile ; "

rootfs_fixup_var_volatile () {
	install -m 1777 -d ${IMAGE_ROOTFS}/${localstatedir}/volatile/tmp
	install -m 755  -d ${IMAGE_ROOTFS}/${localstatedir}/volatile/log
}

