# we might try somethimg like that as well
# https://github.com/nefethael/meta-random/tree/zeus/recipes-dbs/influxdb

DESCRIPTION = "InfluxDB is an open source time series platform. This includes APIs for storing and querying data,  \
               processing it in the background for ETL or monitoring and alerting purposes, user dashboards,       \
               and visualizing and exploring the data and more.                                                    \
               The master branch on this repo now represents the latest InfluxDB, which now includes functionality \
               for Kapacitor (background processing) and Chronograf (the UI) all in a single binary."

GO_IMPORT = "github.com/influxdata/influxdb"
SRC_URI = "git://${GO_IMPORT};branch=1.8"
SRCREV = "781490de48220d7695a05c29e5a36f550a4568f5"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${WORKDIR}/${PN}-${PV}/src/${GO_IMPORT}/LICENSE;md5=f39a8d10930fb37bd59adabb3b9d0bd6"
inherit go
CGO_ENABLED = "1"

#DEPENDS += "go-dep-native"

#RDEPENDS_${PN}-dev += "python bash"

#RDEPENDS_${PN}-staticdev += "python bash perl make"

#DEPENDS += "github.com-cespare-xxhash"
#DEPENDS += "code.googlesource.com-gocloud"

#RDEPENDS_${PN} += "github.com-cespare-xxhash"

# what about licenses of all the modules included? We only deal with the top level licenses here.

do_compile_prepend() {
    rm -f ${WORKDIR}/build/src/${GO_IMPORT}/Gopkg.toml
    rm -f ${WORKDIR}/build/src/${GO_IMPORT}/Gopkg.lock
    cd ${WORKDIR}/build/src/${GO_IMPORT}
    #dep init
    #dep init
    #dep ensure
    go mod init
}

python do_fix_objs_trampoline() {
    bb.build.exec_func('do_fix_objs', d)
}

# trying to remove some irrelevant obj files, which tools choke on
do_fix_objs() {
   # code.googlesource.com-gocloud/git-r0/packages-split/code.googlesource.com-gocloud-staticdev/usr/lib/go/pkg/dep/sources/
   # https---code.googlesource.com-gocloud/cmd/go-cloud-debug-agent/internal/debug/elf/testdata/
   find ${D}${libdir}/go/pkg -depth -type f -name go-relocation-test-* -exec rm -rf {} \;
   find ${D}${libdir}/go/pkg -depth -type f -name compressed-32.obj -exec rm -rf {} \;
   find ${D}${libdir}/go/pkg -depth -type f -name gcc-386-freebsd-exec -exec rm -rf {} \;

   # QA Issue: Architecture did not match (x86, expected x86-64) on /work/core2-64-resy-linux/code.googlesource.com-gocloud/git-r0/packages-split/
   #           code.googlesource.com-gocloud-staticdev/usr/lib/go/pkg/dep/sources/https---go.googlesource.com-tools/go/internal/gccgoimporter/testdata/v1reflect.gox [arch]
   find ${D}${libdir}/go/pkg -depth -type f -name v1reflect.gox -exec rm -rf {} \;

   # QA Issue: /usr/lib/go/src/github.com/influxdata/influxdb/build.py contained in package github.com-influxdata-influxdb-dev requires /usr/bin/python2.7, 
   # but no providers found in RDEPENDS_github.com-influxdata-influxdb-dev? [file-rdeps]
   # although I added meta-python2 and python to the -dev package

   if [ -f ${D}${libdir}/go/src/github.com/influxdata/influxdb/build.py ]; then
      rm -f ${D}${libdir}/go/src/github.com/influxdata/influxdb/build.py
   fi
   #find ${D}${libdir}/go/src/github.com/influxdata/influxdb -depth -type f -name build.py -exec rm -rf {} \;
}

addtask do_fix_objs_trampoline
addtask do_fix_objs

# looks like until populate_sysroot we are good now
do_install[postfuncs] += "do_fix_objs_trampoline"

