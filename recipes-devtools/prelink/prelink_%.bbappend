# --> This should be excluded from prelink
# so we add at the beginning of prelink.conf:
# grafana:
# -b /usr/bin/grafana-server
# -b /usr/bin/grafana-cli
# 
# <-- This should be excluded from prelink

do_configure_append () {
#         sed -i '1 i\# --> This should be excluded from prelink\n# grafana\n-b \/usr\/bin\/grafana-server\n# <-- This should be excluded from prelink\n' ${WORKDIR}/prelink.conf  
   # only add if grafana-server is not already there     
   sed -zi '/\/usr\/bin\/grafana-server/!s/$/\# --> This should be excluded from prelink\n# grafana\n-b \/usr\/bin\/grafana-server\n-b \/usr\/bin\/grafana-cli\n# <-- This should be excluded from prelink\n/' ${WORKDIR}/prelink.conf
}

BBCLASSEXTEND = "native"
