# Note that busybox is conventient and might be required as well by /bin/sh,
# and the access* modules need to be explicitly specified since RECOMMENDATIONS
# are disabled.
IMAGE_INSTALL += " \
	busybox \
	influxdb \
"
