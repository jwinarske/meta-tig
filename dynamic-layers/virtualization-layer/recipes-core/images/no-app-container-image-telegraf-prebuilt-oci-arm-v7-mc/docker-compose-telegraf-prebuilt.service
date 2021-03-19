[Unit]
Description=docker-compose telegraf-prebuilt service
After=docker.service network-online.target
Requires=docker.service network-online.target

[Service]
WorkingDirectory=/resy-container/docker-compose/telegraf-prebuilt

# with ExecStartPre we import the container from prepopulated file
ExecStartPre=/bin/sh -c "/resy-container/telegraf-prebuilt/create-inflxdb-name.sh && /usr/bin/docker image inspect reliableembeddedsystems/oci-telegraf-prebuilt:latest >/dev/null 2>&1 && echo reliableembeddedsystems/oci-telegraf-prebuilt:latest exists || /usr/bin/docker import /resy-container/prepopulated-container-images/app-container-image-telegraf-prebuilt-oci-container-arm-v7.tar.bz2 reliableembeddedsystems/oci-telegraf-prebuilt:latest"

# --> as up without -d
#Type=simple
#TimeoutStartSec=15min
#Restart=always
#
#ExecStart=/usr/bin/docker-compose up --remove-orphans
#ExecStop=/usr/bin/docker-compose down --remove-orphans
#
# <-- as up without -d

# --> as up with -d
# Detached mode: Run containers in the background,
#                print new container names. Incompatible with
#                --abort-on-container-exit.
#
Type=oneshot
RemainAfterExit=yes

ExecStart=/usr/bin/docker-compose up -d
ExecStop=/usr/bin/docker-compose down

ExecReload=/usr/bin/docker-compose up -d
# <-- as up with -d

[Install]
WantedBy=multi-user.target
