require recipes-core/images/core-image-minimal-base.bb

# dpkg-start-stop is needed by grafana

IMAGE_INSTALL += "\
telegraf \
influxdb \
grafana \
dpkg-start-stop \
"

IMAGE_INSTALL += "\
chronograf \
kapacitor \
"

# playing around with mqtt

IMAGE_INSTALL += "\
mosquitto \
mosquitto-clients \
"

