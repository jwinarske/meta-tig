# Copyright (C) 2021 Robert Berger <robert.berger@ReliableEmbeddedSystems.com>
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "A telegraf image"

require dynamic-layers/virtualization-layer/recipes-core/images/app-container-image-oci.bb
require recipes-core/images/app-container-image-telegraf-prebuilt-common.inc
