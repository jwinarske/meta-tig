DESCRIPTION = "Go packages for Google Cloud Platform services \
              "

GO_IMPORT = "code.googlesource.com/gocloud"
SRC_URI = "git://${GO_IMPORT};branch=master;protocol=https;"

# v0.58.0
SRCREV = "4ce401d35e3bca0758067449fc7a326aa94f673b"
LICENSE = "Apache-2"
LIC_FILES_CHKSUM = "file://${WORKDIR}/${PN}-${PV}/src/${GO_IMPORT}/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57"
inherit go

DEPENDS += "go-dep-native"

RDEPENDS_${PN}-staticdev += "bash"
RDEPENDS_${PN}-dev += "bash"

do_compile_prepend() {
    rm -f ${WORKDIR}/build/src/${GO_IMPORT}/Gopkg.toml
    rm -f ${WORKDIR}/build/src/${GO_IMPORT}/Gopkg.lock
    cd ${WORKDIR}/build/src/${GO_IMPORT}
    dep init
    #dep ensure -vendor-only
    dep ensure
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
}

addtask do_fix_objs_trampoline
addtask do_fix_objs

# looks like until populate_sysroot we are good now
do_install[postfuncs] += "do_fix_objs_trampoline"
