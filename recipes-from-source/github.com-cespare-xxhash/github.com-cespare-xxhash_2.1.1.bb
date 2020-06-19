DESCRIPTION = "A Go implementation of the 64-bit xxHash algorithm (XXH64) \ 
              "
GO_IMPORT = "github.com/cespare/xxhash"
SRC_URI = "git://${GO_IMPORT};branch=master;protocol=https;"

SRCREV = "d7df74196a9e781ede915320c11c378c1b2f3a1f"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${WORKDIR}/${PN}-${PV}/src/${GO_IMPORT}/LICENSE.txt;md5=802da049c92a99b4387d3f3d91b00fa9"
inherit go

DEPENDS += "go-dep-native"

do_compile_prepend() {
    rm -f ${WORKDIR}/build/src/${GO_IMPORT}/Gopkg.toml
    rm -f ${WORKDIR}/build/src/${GO_IMPORT}/Gopkg.lock
    cd ${WORKDIR}/build/src/${GO_IMPORT}
    dep init
    #dep ensure -vendor-only
    dep ensure
}

