DESCRIPTION = "Kapacitor"
SUMMARY = "Kapacitor is an open source framework for processing, monitoring, and alerting on time series data."
HOMEPAGE = "https://www.influxdata.com/time-series-platform/kapacitor/"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://src/${GO_IMPORT}/LICENSE;md5=96cd9a86f733dbfec4107613b9b27c71"

GO_IMPORT = "github.com/influxdata/kapacitor"

#GOBUILDFLAGS:remove = "-buildmode=pie"
# don't check out modules read only - se we can clean/cleanall
GOBUILDFLAGS:append = " -modcacherw"
inherit go go-mod

BRANCH = "nobranch=1"
# this is tag v1.6.4:
#SRCREV = "dfdea23b82343fca1976358b9d98cd8ec42e09df"
# this is tag v1.5.9:
SRCREV = "06a16e51ceb5b7086b3b855969c3f93532da1550"

SRC_URI = "\
           git://github.com/influxdata/kapacitor;${BRANCH};protocol=https;destsuffix=${BPN}-${PV}/src/${GO_IMPORT} \
          "
# We need go-native for ${GO} underneath
# I thought that "inherit go" would take care about it
# but apparently it does not
do_compile[depends] += "go-native:do_populate_sysroot"

export CGO_ENABLED="1"

# ERROR: github.com-influxdata-kapacitor-1.6.4-r0 do_populate_sysroot: QA Issue: : /work/armv7at2hf-neon-resy-linux-gnueabi/github.com-influxdata-kapacitor/1.6.4-r0/sysroot-destdir/usr/lib/go/pkg/mod/github.com/influxdata/flux@v0.151.1/libflux/flux-core/src/scanner/scanner.rl maximum shebang size exceeded, the maximum size is 128. [shebang-size]
# ERROR: github.com-influxdata-kapacitor-1.6.4-r0 do_populate_sysroot: QA Issue: : /work/armv7at2hf-neon-resy-linux-gnueabi/github.com-influxdata-kapacitor/1.6.4-r0/sysroot-destdir/usr/lib/go/pkg/mod/github.com/influxdata/flux@v0.151.1/libflux/flux-core/src/scanner/scanner_generated.rs maximum shebang size exceeded, the maximum size is 128. [shebang-size]
# hack:
# INSANE_SKIP:${PN}:append = " shebang-size"

# custom do_compile, which is similar to building it
# from commandline
#do_compile() {
#        # Pass the needed cflags/ldflags so that cgo
#        # can find the needed headers files and libraries
#        export CGO_ENABLED="1"
#        # Pass the needed cflags/ldflags so that cgo
#        # can find the needed headers files and libraries
#        export GOARCH=${TARGET_GOARCH}
#        export CGO_ENABLED="1"
#        export CGO_CFLAGS="${CFLAGS} --sysroot=${STAGING_DIR_TARGET}"
#        export CGO_LDFLAGS="${LDFLAGS} --sysroot=${STAGING_DIR_TARGET}"
#        export GO111MODULE=on
#
#        ${GO} build ./cmd/kapacitord
#        ${GO} build ./cmd/kapacitor
#}

# some var which will be used afterwards
SRC_ROOT="${S}/src/${GO_IMPORT}"

# custom do_compile:
#       1. only fetch 
#       2. fix permissions of pkg/mod/xxx directories
#       3. modifed version of do_compile, taken from bbclass
#do_compile() {
#        # let's try not to build, but just download the stuff
#        # go -d does not build, but just download
#        ${GO} get -d github.com/influxdata/telegraf/cmd/telegraf
#
#        # --> fix permissions
#        if [ -d ${B}/pkg/mod ]; then
#        find ${B}/pkg/mod -depth -type d -name go.opencensus.io* -exec chmod -R 755 {} \;
#        find ${B}/pkg/mod -depth -type d -name collectd.org* -exec chmod -R 755 {} \;
#        find ${B}/pkg/mod -depth -type d -name go.starlark.net* -exec chmod -R 755 {} \;
#        find ${B}/pkg/mod -depth -type d -name modernc.org* -exec chmod -R 755 {} \;
#        find ${B}/pkg/mod -depth -type d -name k8s.io* -exec chmod -R 755 {} \;
#        find ${B}/pkg/mod -depth -type d -name cloud.google.com -exec chmod -R 755 {} \;
#        find ${B}/pkg/mod -depth -type d -name code.cloudfoundry.org -exec chmod -R 755 {} \;
#        find ${B}/pkg/mod -depth -type d -name github.com -exec chmod -R 755 {} \;
#        find ${B}/pkg/mod -depth -type d -name golang.org -exec chmod -R 755 {} \;
#        find ${B}/pkg/mod -depth -type d -name golang.zx2c4.com -exec chmod -R 755 {} \;
#        find ${B}/pkg/mod -depth -type d -name google.golang.org -exec chmod -R 755 {} \;
#        find ${B}/pkg/mod -depth -type d -name gopkg.in -exec chmod -R 755 {} \;
#        fi
#        # <-- fix permissions   
#
#        # --> this is a modified version of do_compile from the bbclass
#        export TMPDIR="${GOTMPDIR}"
#        if [ -n "${GO_INSTALL}" ]; then
#                if [ -n "${GO_LINKSHARED}" ]; then
#                        ${GO} install ${GOBUILDFLAGS} github.com/influxdata/telegraf/cmd/telegraf
#                        rm -rf ${B}/bin
#                fi
#                ${GO} install ${GO_LINKSHARED} ${GOBUILDFLAGS} github.com/influxdata/telegraf/cmd/telegraf
#        fi
#        # <-- this is a modified version of do_compile from the bbclass
#}


#do_install:append() {
#	# --> etc
#    	# /etc
#    	install -d ${D}${sysconfdir}/logrotate.d
#    	install -d ${D}${sysconfdir}/telegraf
#    	install -d ${D}${sysconfdir}/telegraf/telegraf.d
#
#        install -m 0644 ${SRC_ROOT}/etc/logrotate.d/telegraf ${D}${sysconfdir}/logrotate.d/
#        install -m 0644 ${SRC_ROOT}/etc/telegraf.conf ${D}${sysconfdir}/telegraf/
#	# <-- etc
#
#	# --> sysvinit
#    	# Only install the script if 'sysvinit' is in DISTRO_FEATURES
#	# systemd would be the other choice
#    	if ${@bb.utils.contains('DISTRO_FEATURES','sysvinit','true','false',d)}; then
#           install -d ${D}${sysconfdir}/init.d/
#	   # not sure we always have bash, so we replace it with /bin/sh
#	   # please note, that below we created a telegraf user/group for this to work
#           sed -i 's,#! /usr/bin/env bash,#! /bin/sh,g' ${SRC_ROOT}/scripts/init.sh
#           install -D -m 0755 ${SRC_ROOT}/scripts/init.sh ${D}${sysconfdir}/init.d/telegraf
#    	fi
#	#  <-- sysvinit
#
#	# --> systemd
#    	# Only install the script if 'systemd' is in DISTRO_FEATURES
#    	# systemd
#    	if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd','true','false',d)}; then
#           install -d ${D}${systemd_unitdir}/system
#	   # please note, that below we created a telegraf user/group for this to work
#           install -m 0644 ${SRC_ROOT}/scripts/telegraf.service ${D}${systemd_unitdir}/system/telegraf.service
#        fi
#	# <-- systemd 
#
#	# /var - for /var/log/telegraf/telegraf.log
#    	install -d ${D}${localstatedir}/log/telegraf
#
# ERROR: github.com-influxdata-telegraf-1.18.0-r0 do_package_qa: QA Issue: /usr/lib/go/pkg/mod/github.com/docker/libnetwork@at@v0.8.0-dev.2.0.20181012153825-d7b61745d166/cmd/ssd/ssd.py contained in package github.com-influxdata-telegraf-staticdev requires /usr/bin/python, but no providers found in RDEPENDS:github.com-influxdata-telegraf-staticdev? [file-rdeps]
#
#         # we have python3 now and not python2, so
#         # .from /usr/bin/python --> /usr/bin/env python:
#         sed -i -e "s%/usr/bin/python*%/usr/bin/env python%g" ${D}${libdir}/go/pkg/mod/github.com/docker/libnetwork@v0.8.0-dev.2.0.20181012153825-d7b61745d166/cmd/ssd/ssd.py
#
# ERROR: github.com-influxdata-telegraf-1.18.0-r0 do_package_qa: QA Issue: /usr/lib/go/pkg/mod/github.com/docker/docker@at@v17.12.0-ce-rc1.0.20200916142827-bd33bbf0497b+incompatible/contrib/init/openrc/docker.initd contained in package github.com-influxdata-telegraf-staticdev requires /sbin/openrc-run, but no providers found in RDEPENDS:github.com-influxdata-telegraf-staticdev? [file-rdeps]
#
#       # this openrc seems to be yet another init system
#       # .there seems to be even a meta layer for it
#       # .but currently I don't plan to support it, so remove references to it
#       rm -rf ${D}${libdir}/go/pkg/mod/github.com/docker/docker@v17.12.0-ce-rc1.0.20200916142827-bd33bbf0497b+incompatible/contrib/init/openrc
#}

# fix a qa issue:
RDEPENDS:${PN}-dev += "\
                       python3 \
                       bash \
                       "
# fix qa issues:
# .we might remove some tests/things to get rid 
# .of these dependencies as well
# .see above - end of do_install:append()
# PTEST_ENABLED="0" - does nothing
#RDEPENDS:${PN}-staticdev += "\
#                       bash \
#                       make \
#                       perl \
#                       gawk \
#                       python3 \
#                       "

# --> we need a more complete ps for init script
RDEPENDS:${PN} += "procps"
# <-- we need a more complete ps for init script

# --> user/group
#inherit useradd

# create telegraf group
# --system                  create a system account
#GROUPADD_PARAM:${PN} = "--system telegraf"

# create telegraf user
# --system                  create a system account
# --gid GROUP               name or ID of the primary group of the new
#                           account
#USERADD_PARAM:${PN} += "telegraf \
#                        --system \
#                        --gid telegraf \
#                       "
# USERADD_PACKAGES specifies the output packages
# which include custom users/groups.
#USERADD_PACKAGES = "${PN}"
# <-- user/group

# --> systemd service
# please note, that above we already copy files depeding on sysvinit/systemd
#REQUIRED_DISTRO_FEATURES= "systemd"
#inherit systemd
#SYSTEMD_AUTO_ENABLE = "enable"
# disable for manual testing
# e.g. on target:
# systemctl start telegraf.service
##SYSTEMD_AUTO_ENABLE = "disable"
#SYSTEMD_PACKAGES = "${PN}"
#SYSTEMD_SERVICE:${PN} = "telegraf.service"
# <-- systemd service

# --> sysvinit scripts
# please note, that above we already copy files depeding on sysvinit/systemd
#INITSCRIPT_PACKAGES = "${PN}"
#INITSCRIPT_NAME:${PN} = "telegraf"
# script has a runlevel of: 99
# starts in initlevels:     2 3 4 5
# stops  in initlevels: 0 1         6
#INITSCRIPT_PARAMS:${PN} = "start 99 2 3 4 5 . stop 99 0 1 6 ."
# <-- sysvinit scripts

### temporarily only for license experiments begin
# --> license detector
# do_devshell[depends] += "github.com-google-go-license-detector-native:do_populate_sysroot"
# <-- license detector
### temporarily only for license experiments end
