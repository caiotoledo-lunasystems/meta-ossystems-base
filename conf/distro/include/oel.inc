DISTRO_VERSION = "16.2"

SDK_VENDOR = "-oelsdk"
SDK_VERSION := "${@'${DISTRO_VERSION}'.replace('snapshot-${DATE}','snapshot')}"
SDK_NAME_PREFIX = "${DISTRO}"

MAINTAINER = "O.S. Systems Software LTDA. <contato@ossystems.com.br>"

TARGET_VENDOR = "-oel"

LOCALCONF_VERSION = "1"

# Target i686 for the sdk binaries, rather than the build arch.
#
# Other options: i586, x86_64
SDKMACHINE ?= "x86_64"

# Paths
OELDIR ?= "${COREBASE}/.."
DEPLOY_DIR_IMAGE = "${DEPLOY_DIR}/images/${MACHINE}"
SDKPATH = "/opt/${DISTRO}/${SDK_VERSION}"

# Move the persist db up out of TMPDIR so it survives its removal
PERSISTENT_DIR = "${TOPDIR}/db"

# Restore any available saved headrevs
DUMP_HEADREVS_DB ?= '${OELDIR}/${MACHINE}/saved_persist_data.db'
INHERIT += "restore-dumped-headrevs"

# LAYERDIRs for our layers, by name
INHERIT += "layerdirs"

# Do an up front type check to sanity check user configuration
INHERIT += "typecheck"

# QA check settings - a little stricter than the OE-Core defaults
WARN_TO_ERROR_QA = "already-stripped compile-host-path install-host-path \
                    installed-vs-shipped ldflags pn-overrides rpaths staticdev \
                    useless-rpaths host-user-contaminated"
WARN_QA_remove = "${WARN_TO_ERROR_QA}"
ERROR_QA_append = " ${WARN_TO_ERROR_QA}"

# Also check for host user contamination in the rootfs
ROOTFS_POSTPROCESS_COMMAND_append = " rootfs_check_host_user_contaminated;"

SANITY_TESTED_DISTROS ?= " \
			Ubuntu-14.04 \n \
			Ubuntu-14.10 \n \
			Ubuntu-15.04 \n \
			Ubuntu-15.10 \n \
			Ubuntu-16.04 \n \
			CentOSLinux-7.* \n \
			Debian-8.* \n \
"

#
# OELAYOUT_ABI allows us to notify users when the format of TMPDIR changes in
# an incompatible way. Such changes should usually be detailed in the commit
# that breaks the format and have been previously discussed on the mailing list
# with general agreement from the core team.
#
OELAYOUT_ABI = "11"

DISTRO_EXTRA_RDEPENDS += " ${OEL_DEFAULT_EXTRA_RDEPENDS}"
DISTRO_EXTRA_RRECOMMENDS += " ${OEL_DEFAULT_EXTRA_RRECOMMENDS}"

####
#### Distro Features & Recipe Configuration
####

# Override these in oel based distros
OEL_DEFAULT_DISTRO_FEATURES = "largefile opengl multiarch argp xattr"
OEL_DEFAULT_EXTRA_RDEPENDS = "packagegroup-core-boot"
OEL_DEFAULT_EXTRA_RRECOMMENDS = ""

DISTRO_FEATURES ?= "${DISTRO_FEATURES_LIBC} \
                    ${OEL_DEFAULT_DISTRO_FEATURES}"

# Exclude DATE from DISTRO/SDK_VERSION checksums
DISTRO_VERSION[vardepsexclude] = "DATE"
SDK_VERSION[vardepsexclude] = "DATE"

# Lower the priority of meta-oe, as we prefer oe-core when recipes are duped
BBFILE_PRIORITY_openembedded-layer = "1"

# locales shouldn't be in lib32/lib64
localedir = "${exec_prefix}/lib/locale"

QEMU_TARGETS ?= "arm aarch64 i386 x86_64"

# Preferred kernel version for QEMU based machines
PREFERRED_VERSION_linux-yocto ?= "4.8%"
PREFERRED_VERSION_linux-yocto_qemux86 ?= "4.8%"
PREFERRED_VERSION_linux-yocto_qemux86-64 ?= "4.8%"
PREFERRED_VERSION_linux-yocto_qemuarm ?= "4.8%"
PREFERRED_VERSION_linux-yocto_qemumips ?= "4.8%"
PREFERRED_VERSION_linux-yocto_qemumips64 ?= "4.8%"
PREFERRED_VERSION_linux-yocto_qemuppc ?= "4.8%"

OELQEMUDEPS = "${@bb.utils.contains("INCOMPATIBLE_LICENSE", "GPLv3", "", "packagegroup-core-device-devel",d)}"
DISTRO_EXTRA_RDEPENDS_append_qemuarm = " ${OELQEMUDEPS}"
DISTRO_EXTRA_RDEPENDS_append_qemuarm64 = " ${OELQEMUDEPS}"
DISTRO_EXTRA_RDEPENDS_append_qemumips = " ${OELQEMUDEPS}"
DISTRO_EXTRA_RDEPENDS_append_qemuppc = " ${OELQEMUDEPS}"
DISTRO_EXTRA_RDEPENDS_append_qemux86 = " ${OELQEMUDEPS}"
DISTRO_EXTRA_RDEPENDS_append_qemux86-64 = " ${OELQEMUDEPS}"

TCLIBCAPPEND = ""

# Use a local PR server by default
PRSERV_HOST ?= "localhost:0"

####
#### Inherits
####

# Remove .la files for packages, as they really aren't of much use.
DISTRO_FEATURES_BACKFILL_CONSIDERED += "libtool-garbage"
INHERIT += "libtool-garbage"

# We want information about image contents
INHERIT += "buildhistory"
BUILDHISTORY_DIR ?= "${TOPDIR}/buildhistory"
BUILDHISTORY_COMMIT ?= "1"

# Do an up front type check to sanity check user configuration
INHERIT += "typecheck"

# Allow blacklisting recipes
INHERIT += "blacklist"

# Copy the image license manifest into DEPLOY_DIR_IMAGE
INHERIT += "deploy-license-manifest"

# Ensure we have what we need for the below type checks
OE_IMPORTS += "oe.terminal"

# Restore any available saved headrevs (used for our installers)
DUMP_HEADREVS_DB ?= "${MELDIR}/${MACHINE}/saved_persist_data.db"
INHERIT += "restore-dumped-headrevs"

####
#### Preferences and providers
####

# Prefer normal jpeg implementation
PREFERRED_PROVIDER_jpeg               ??= "jpeg"
PREFERRED_PROVIDER_jpeg-native        ??= "jpeg-native"

# Prefer O.S. Systems U-Boot fork
PREFERRED_PROVIDER_u-boot              ?= "u-boot-ossystems"
PREFERRED_PROVIDER_virtual/bootloader  ?= "u-boot-ossystems"

# Prefer the chkconfig C implementation of alternatives
VIRTUAL-RUNTIME_update-alternatives = "chkconfig-alternatives"
PREFERRED_PROVIDER_virtual/update-alternatives = "chkconfig-alternatives"
PACKAGECONFIG_remove_pn-opkg-utils = "update-alternatives"
PREFERRED_PROVIDER_virtual/update-alternatives-native = "chkconfig-alternatives-native"
PACKAGECONFIG_remove_pn-opkg-utils-native = "update-alternatives"
PREFERRED_PROVIDER_virtual/nativesdk-update-alternatives = "nativesdk-chkconfig-alternatives"
PACKAGECONFIG_remove_pn-nativesdk-opkg-utils = "update-alternatives"

# Ensure that we are not getting GUI for latencytop, as we don't want x11 to
# be pulled into our console image.
PACKAGECONFIG_pn-latencytop = ""

# Ensure that we are not getting GUI for sysprof, as we don't want x11 to
# be pulled into our console image.
PACKAGECONFIG_pn-sysprof = ""

# Enable systemd in pulseaudio when appropriate
PACKAGECONFIG_append_pn-pulseaudio = "${@bb.utils.contains('DISTRO_FEATURES', 'systemd', ' systemd', '', d)}"

# Since many embedded systems don't have non-root users, allow autospawn for
# root as well.
PACKAGECONFIG_append_pn-pulseaudio = " autospawn-for-root"

# Ensure we have the writable paths we need in a read-only rootfs
VOLATILE_BINDS_append = "\
	/var/volatile/root-home ${ROOT_HOME}\n\
	/var/volatile/media /media\n\
	/var/volatile/resolv.conf /etc/resolv.conf\n\
"

# Override pulseaudio to use a system rather than user systemd service when we
# don't have pam available, as the user service won't be started
systemd_userunitdir_pn-pulseaudio = "${@bb.utils.contains('DISTRO_FEATURES', 'pam', '${systemd_unitdir}/user', '${systemd_unitdir}/system', d)}"

# We prefer busybox rather than tinylogin
VIRTUAL-RUNTIME_login_manager = "busybox"

# Default to these image types
IMAGE_FSTYPES ?= "tar.bz2 ext3"
UBI_VOLNAME = "rootfs"

# Quadruple the normal. 'du' is not a good way to really see how much
# space will be needed and fails badly as the fs size grows.
IMAGE_ROOTFS_EXTRA_SPACE = "40960"

# Sane default locales for images
GLIBC_GENERATE_LOCALES ?= "en_US en_US.UTF-8"
IMAGE_LINGUAS ?= "en-us"

# Include nss-myhostname for sysvinit, so the hostname resolves. systemd
# includes myhostname itself.
DISTRO_EXTRA_RRECOMMENDS += "${@bb.utils.contains('DISTRO_FEATURES', 'systemd', '', 'nss-myhostname', d)}"

####
#### Workarounds & Overrides
####

# Work around missing vardep bug in bitbake
sstate_stage_all[vardeps] += "sstate_stage_dirs"

# Restore any available saved headrevs (used for our installers)
DUMP_HEADREVS_DB ?= "${MELDIR}/${MACHINE}/saved_persist_data.db"
INHERIT += "restore-dumped-headrevs"

# LAYERDIRs for our layers, by name
INHERIT += "layerdirs"

# Layer name and path for the recipe
RECIPE_LAYERNAME = "${@bb.utils.get_file_layer('${FILE}', d) or 'UNKNOWN'}"
RECIPE_LAYERDIR = "${LAYERDIR_${RECIPE_LAYERNAME}}"
LAYERDIR_UNKNOWN = 'UNKNOWN'

# Support pulling downloads and sstate from inside individual layers. This
# will let us ship self contained layers to a release without risking file
# conflicts between them.
PREMIRRORS_prepend = "${@'.*://.*/.* file://${RECIPE_LAYERDIR}/downloads\n' if '${RECIPE_LAYERDIR}' != 'UNKNOWN' else ''}"
LAYER_SSTATE_MIRRORS = "${@" ".join('file://%s' % sl for sl in ('%s/sstate-cache' % l for l in '${BBLAYERS}'.split()) if os.path.exists(sl))}"
SSTATE_MIRROR_SITES_prepend = "${LAYER_SSTATE_MIRRORS} "

####
#### Includes
####

# Ensure that we implement shared state reuse handling for non-target recipes
require conf/distro/include/sstate.inc

# Disable static libs by default
require conf/distro/include/no-static-libs.inc

# Enable uninative
require conf/distro/include/yocto-uninative.inc
INHERIT += "uninative"