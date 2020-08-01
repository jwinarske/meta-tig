DESCRIPTION = "InfluxDB"
SUMMARY = "InfluxDB is a time series database designed to handle high write and query loads."
HOMEPAGE = "https://www.influxdata.com/products/influxdb-overview/"

INSANE_SKIP_${PN}_append = "already-stripped"

do_install() {
    #${sysconfdir} = /etc
    #${bindir} = /usr/bin
    #${libdir} = /usr/lib
    #${datadir} = /usr/share
    #${localstatedir} = /var

    # /etc
    install -d ${D}${sysconfdir}/influxdb
    install -d ${D}${sysconfdir}/logrotate.d

    # x86-64 and armv7 have influxdb.conf at different places
    if [ -f ${S}/etc/influxdb/influxdb.conf ]; then
      install -m 0644 ${S}/etc/influxdb/influxdb.conf ${D}${sysconfdir}/influxdb/
    else
      install -m 0644 ${S}/influxdb.conf ${D}${sysconfdir}/influxdb/ 
    fi 
 
    # armv7 has this file, x86-64 not 
    # @@@ TODO: fix it
    if [ -f  ${S}/etc/logrotate.d/influxdb ]; then
    install -m 0644 ${S}/etc/logrotate.d/influxdb ${D}${sysconfdir}/logrotate.d/
    fi

    # /usr/bin
    install -d ${D}${bindir}

    # arm has a bin dir
    if [ -d ${S}/usr/bin/ ]; then
    install -m 0755 ${S}/usr/bin/* ${D}${bindir}/
    else
    # x86-64 does not
    install -m 0755 ${S}/influx ${D}${bindir}/
    install -m 0755 ${S}/influxd ${D}${bindir}/
    install -m 0755 ${S}/influx_inspect ${D}${bindir}/
    install -m 0755 ${S}/influx_stress ${D}${bindir}/
    install -m 0755 ${S}/influx_tsm ${D}${bindir}/
    fi

    # /usr/lib
    if [ -d ${S}/usr/lib ]; then
    # only form armv7
    install -d ${D}${systemd_unitdir}/system
    sed -i 's/User=influxdb/User=root/g' ${S}/usr/lib/influxdb/scripts/influxdb.service
    sed -i 's/Group=influxdb/Group=root/g' ${S}/usr/lib/influxdb/scripts/influxdb.service
    install -m 0644 ${S}/usr/lib/influxdb/scripts/influxdb.service ${D}${systemd_unitdir}/system
    #install -m 0644 ${S}/usr/lib/influxdb/scripts/influxdb.service ${D}${libdir}/influxdb/scripts/
    #install -m 0644 ${S}/usr/lib/influxdb/scripts/init.sh ${D}${libdir}/influxdb/scripts/
    fi

    # /usr/share
    install -d ${D}${datadir}/man/man1
    install -m 0644 ${S}/usr/share/man/man1/* ${D}${datadir}/man/man1/

    # /var/lib
    install -d ${D}${localstatedir}/lib/influxdb
    install -d ${D}${localstatedir}/log/influxdb

}
inherit systemd
SYSTEMD_SERVICE_${PN} = "influxdb.service"

# x86-64 static
# ├── influx
# ├── influxd
# ├── influxdb.conf
# ├── influx_inspect
# ├── influx_stress
# ├── influx_tsm
# └── usr
#     └── share
#         └── man
#             └── man1
#                 ├── influx.1.gz
#                 ├── influxd.1.gz
#                 ├── influxd-backup.1.gz
#                 ├── influxd-config.1.gz
#                 ├── influxd-restore.1.gz
#                 ├── influxd-run.1.gz
#                 ├── influxd-version.1.gz
#                 ├── influx_inspect.1.gz
#                 ├── influx_stress.1.gz
#                 └── influx_tsm.1.gz

# armv7:
# .
# ├── etc
# │   ├── influxdb
# │   │   └── influxdb.conf
# │   └── logrotate.d
# │       └── influxdb
# ├── usr
# │   ├── bin
# │   │   ├── influx
# │   │   ├── influxd
# │   │   ├── influx_inspect
# │   │   ├── influx_stress
# │   │   └── influx_tsm
# │   ├── lib
# │   │   └── influxdb
# │   │       └── scripts
# │   │           ├── influxdb.service
# │   │           └── init.sh
# │   └── share
# │       └── man
# │           └── man1
# │               ├── influx.1.gz
# │               ├── influxd.1.gz
# │               ├── influxd-backup.1.gz
# │               ├── influxd-config.1.gz
# │               ├── influxd-restore.1.gz
# │               ├── influxd-run.1.gz
# │               ├── influxd-version.1.gz
# │               ├── influx_inspect.1.gz
# │               ├── influx_stress.1.gz
# │               └── influx_tsm.1.gz
# └── var
#     ├── lib
#     │   └── influxdb
#     └── log
#         └── influxdb

