#!/bin/sh
UNIQUE_ID="$(/resy-container/telegraf-prebuilt/device-identity)"
#echo "UNIQUE_ID: ${UNIQUE_ID}"

# read current influxdb database string, probably first time
DB_ID="$(cat /resy-container/telegraf-prebuilt/etc/telegraf/telegraf.conf | grep '^  # database = \"')"
#echo "DB_ID: ${DB_ID}"

# in case this is empty try again - without the comment, probably not first time 
if [[ -z ${DB_ID} ]]; then
   DB_ID="$(cat /resy-container/telegraf-prebuilt/etc/telegraf/telegraf.conf | grep '^  database = \"')"
fi

#echo "DB_ID: ${DB_ID}"

# remove prefix from string
DB_ID=${DB_ID#"  # database = \""}
#echo "${DB_ID}"

# remove prefix also without a comment from string
DB_ID=${DB_ID#"  database = \""}
#echo "${DB_ID}"

# remove suffix from string
DB_ID="${DB_ID%\"}"
#echo "${DB_ID}"

# sed -i "s,^  # database = \"telegraf\",database = \"$(/resy-container/telegraf/device-identity)\"," telegraf.conf
#/bin/sh -c "/usr/bin/docker image inspect reliableembeddedsystems/oci-lighttpd:latest >/dev/null 2>&1 && echo reliableembeddedsystems/oci-lighttpd:latest exists || /usr/bin/docker import /resy-container/prepopulated-container-images/app-container-image-lighttpd-oci-container-arm-v7.tar.bz2 reliableembeddedsystems/oci-lighttpd:latest"

if [[ "${UNIQUE_ID}" == "${DB_ID}" ]]; then
    echo "Match" 
    echo "${DB_ID}"
    exit 0
else
    echo "No Match - trying to fix"
    sed -i "s,^  # database = \"telegraf\",  database = \"$(/resy-container/telegraf-prebuilt/device-identity)\"," /resy-container/telegraf-prebuilt/etc/telegraf/telegraf.conf
    DB_ID="$(cat /resy-container/telegraf-prebuilt/etc/telegraf/telegraf.conf | grep '^  database = \"')"
    DB_ID=${DB_ID#"  database = \""}
    DB_ID="${DB_ID%\"}"
fi

if [[ "${UNIQUE_ID}" == "${DB_ID}" ]]; then
    echo "Match"
    echo "${DB_ID}"
else
    echo "No Match - something is wrong"
    echo "UNIQUE_ID: ${UNIQUE_ID}"
    echo "DB_ID: ${DB_ID}"
fi
