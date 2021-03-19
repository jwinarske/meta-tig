DESCRIPTION = "Telegraf"
SUMMARY = "Telegraf is the open source server agent to help you collect metrics from your stacks, sensors and systems."
HOMEPAGE = "https://www.influxdata.com/time-series-platform/telegraf/"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://src/${GO_IMPORT}/LICENSE;md5=c5d3aeddd4f7a4c4993bbdc4a41aec44"

GO_IMPORT = "github.com/influxdata/telegraf"

# don't check out modules read only - se we can clean/cleanall
GOBUILDFLAGS_append = " -modcacherw"
inherit go go-mod

BRANCH = "nobranch=1"
SRCREV = "ac5c7f6a1a7402b6825857939fb02014300cb16a"

SRC_URI = "\
           git://github.com/influxdata/telegraf;${BRANCH};protocol=https;destsuffix=${BPN}-${PV}/src/${GO_IMPORT} \
          "
# We need go-native for ${GO} underneath
# I thought that "inherit go" would take care about it
# but apparently it does not
do_compile[depends] += "go-native:do_populate_sysroot"

export CGO_ENABLED="1"

# custom do_compile, which is similar to building it
# from commandline
do_compile() {
        ${GO} build ./cmd/telegraf
}

# some var which will be used afterwards
SRC_ROOT="${S}/src/${GO_IMPORT}"

do_install_append() {
	# --> etc
    	# /etc
    	install -d ${D}${sysconfdir}/logrotate.d
    	install -d ${D}${sysconfdir}/telegraf
    	install -d ${D}${sysconfdir}/telegraf/telegraf.d

        install -m 0644 ${SRC_ROOT}/etc/logrotate.d/telegraf ${D}${sysconfdir}/logrotate.d/
        install -m 0644 ${SRC_ROOT}/etc/telegraf.conf ${D}${sysconfdir}/telegraf/
	# <-- etc

        # --> telegraf exe
        # /usr/bin
        install -d ${D}${bindir}

        # if I build it this funny way (as by hand) the exe ends up here:
        install -m 0755 ${SRC_ROOT}/telegraf ${D}${bindir}/
        # <-- telegraf exe

	# --> sysvinit
    	# Only install the script if 'sysvinit' is in DISTRO_FEATURES
	# systemd would be the other choice
    	if ${@bb.utils.contains('DISTRO_FEATURES','sysvinit','true','false',d)}; then
           install -d ${D}${sysconfdir}/init.d/
	   # not sure we always have bash, so we replace it with /bin/sh
	   # please note, that below we created a telegraf user/group for this to work
           sed -i 's,#! /usr/bin/env bash,#! /bin/sh,g' ${SRC_ROOT}/scripts/init.sh
           install -D -m 0755 ${SRC_ROOT}/scripts/init.sh ${D}${sysconfdir}/init.d/telegraf
    	fi
	#  <-- sysvinit

	# --> systemd
    	# Only install the script if 'systemd' is in DISTRO_FEATURES
    	# systemd
    	if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd','true','false',d)}; then
           install -d ${D}${systemd_unitdir}/system
	   # please note, that below we created a telegraf user/group for this to work
           install -m 0644 ${SRC_ROOT}/scripts/telegraf.service ${D}${systemd_unitdir}/system/telegraf.service
        fi
	# <-- systemd 

	# /var - for /var/log/telegraf/telegraf.log
    	install -d ${D}${localstatedir}/log/telegraf
}

# -> fix permission issues when cleaning

python do_fix_clean_trampoline() {
    bb.build.exec_func('do_fix_clean', d)
}

do_fix_clean() {
   # cleanall has problems to rewove some files here due to permissions
   # this is some brute force fix
   find ${WORKDIR}/build/pkg/mod -depth -type d -name go.opencensus.io* -exec chmod -R 755 {} \;
   find ${WORKDIR}/build/pkg/mod -depth -type d -name collectd.org* -exec chmod -R 755 {} \;
   find ${WORKDIR}/build/pkg/mod -depth -type d -name go.starlark.net* -exec chmod -R 755 {} \;
   find ${WORKDIR}/build/pkg/mod -depth -type d -name modernc.org* -exec chmod -R 755 {} \;
   find ${WORKDIR}/build/pkg/mod -depth -type d -name k8s.io* -exec chmod -R 755 {} \;
   find ${WORKDIR}/build/pkg/mod -depth -type d -name cloud.google.com -exec chmod -R 755 {} \;
   find ${WORKDIR}/build/pkg/mod -depth -type d -name code.cloudfoundry.org -exec chmod -R 755 {} \;
   find ${WORKDIR}/build/pkg/mod -depth -type d -name github.com -exec chmod -R 755 {} \;
   find ${WORKDIR}/build/pkg/mod -depth -type d -name golang.org -exec chmod -R 755 {} \;
   find ${WORKDIR}/build/pkg/mod -depth -type d -name golang.zx2c4.com -exec chmod -R 755 {} \;
   find ${WORKDIR}/build/pkg/mod -depth -type d -name google.golang.org -exec chmod -R 755 {} \;
   find ${WORKDIR}/build/pkg/mod -depth -type d -name gopkg.in -exec chmod -R 755 {} \;
}

addtask do_fix_clean_trampoline
addtask do_fix_clean


# looks like until populate_sysroot we are good now
do_clean[prefuncs] += "do_fix_clean_trampoline"

# <-- fix permission issues when cleaning

# fix a qa issue:
RDEPENDS_${PN}-dev += "\
                       bash \
                       "
# --> we need a more complete ps for init script
RDEPENDS_${PN} += "procps"
# <-- we need a more complete ps for init script

# --> user/group
inherit useradd

# create telegraf group
# --system                  create a system account
GROUPADD_PARAM_${PN} = "--system telegraf"

# create telegraf user
# --system                  create a system account
# --gid GROUP               name or ID of the primary group of the new
#                           account
USERADD_PARAM_${PN} += "telegraf \
                        --system \
                        --gid telegraf \
                       "
# USERADD_PACKAGES specifies the output packages
# which include custom users/groups.
USERADD_PACKAGES = "${PN}"
# <-- user/group

# --> systemd service
# please note, that above we already copy files depeding on sysvinit/systemd
#REQUIRED_DISTRO_FEATURES= "systemd"
inherit systemd
SYSTEMD_AUTO_ENABLE = "enable"
# disable for manual testing
# e.g. on target:
# systemctl start telegraf.service
#SYSTEMD_AUTO_ENABLE = "disable"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "telegraf.service"
# <-- systemd service

# --> sysvinit scripts
# please note, that above we already copy files depeding on sysvinit/systemd
INITSCRIPT_PACKAGES = "${PN}"
INITSCRIPT_NAME_${PN} = "telegraf"
# script has a runlevel of: 99
# starts in initlevels:     2 3 4 5
# stops  in initlevels: 0 1         6
INITSCRIPT_PARAMS_${PN} = "start 99 2 3 4 5 . stop 99 0 1 6 ."
# <-- sysvinit scripts
