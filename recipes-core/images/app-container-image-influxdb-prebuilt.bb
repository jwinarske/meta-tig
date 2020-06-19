# Copyright (C) 2020 Robert Berger <robert.berger@ReliableEmbeddedSystems.com>
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "A influxdb container image"
LICENSE = "MIT"
LIC_FILES_CHECKSUM = "file://${COREBASE}/meta/CONFIG.MIT;md5=1234"

require recipes-core/images/app-container-image.bb
require recipes-core/images/app-container-image-influxdb-prebuilt-common.inc
