#Copyright (C) 2020 Robert Berger <robert.berger@ReliableEmbeddedSystems.com>
# Released under the MIT license (see COPYING.MIT for the terms)

require recipes-core/images/core-image-minimal-virt-docker-ce.bb

IMAGE_INSTALL += "\
		 app-container-image-telegraf-prebuilt-oci-arm-v7-mc \
                 "
