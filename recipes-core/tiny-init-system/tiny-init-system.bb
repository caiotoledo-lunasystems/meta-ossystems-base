SUMMARY = "Basic init system for constrained systems"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "\
    file://init \
    file://rc.local.sample \
"

S = "${WORKDIR}"

do_configure[noexec] = "1"
do_compile[noexec] = "1"

do_install() {
    install -m 0755 ${WORKDIR}/init ${D}
    install -Dm 0755 ${WORKDIR}/rc.local.sample ${D}${sysconfdir}/rc.local.sample
}

FILES_${PN} = "/init ${sysconfdir}/rc.local.sample"
RDEPENDS_${PN} = "busybox"
