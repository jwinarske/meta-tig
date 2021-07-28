require recipes-core/images/core-image-minimal-base.bb

IMAGE_INSTALL += "\
telegraf \
influxdb \
grafana \
dpkg-start-stop \
"

# dpkg-start-stop is needed by grafana
