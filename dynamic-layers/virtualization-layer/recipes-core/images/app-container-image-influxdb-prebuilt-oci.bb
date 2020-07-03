# Copyright (C) 2020 Robert Berger <robert.berger@ReliableEmbeddedSystems.com>
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "An influxdb image"

require dynamic-layers/virtualization-layer/recipes-core/images/app-container-image-oci.bb
require recipes-core/images/app-container-image-influxdb-prebuilt-common.inc
