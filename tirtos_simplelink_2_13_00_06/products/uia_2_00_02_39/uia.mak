#
#  ======== uia.mak ========
#

#
# Where to install/stage the packages
# Typically this would point to the devkit location
#
DESTDIR ?= <UNDEFINED>

packagesdir ?= /packages
libdir ?= /lib
includedir ?= /include

ifeq ($(docdir),)
docdir := /share/ti/uia/doc
packagedocdir := /docs/uia
else
packagedocdir := $(docdir)
endif

ifeq ($(prefix),)
prefix := /usr
packageprefix := /
else
packageprefix := $(prefix)
endif

# Set up dependencies
XDC_INSTALL_DIR ?=
BIOS_INSTALL_DIR ?=
IPC_INSTALL_DIR ?=
NDK_INSTALL_DIR ?=

#
# Set location of various cgtools
# These variables can be set here or on the command line.
#
ti.targets.C28_large ?=
ti.targets.C28_float ?=
ti.targets.C64P ?=
ti.targets.C64P_big_endian ?=
ti.targets.C64T ?=
ti.targets.C674 ?=

ti.targets.elf.C64P ?=
ti.targets.elf.C64P_big_endian ?=
ti.targets.elf.C64T ?=
ti.targets.elf.C66 ?=
ti.targets.elf.C66_big_endian ?=
ti.targets.elf.C674 ?=

ti.targets.arm.elf.Arm9 ?=
ti.targets.arm.elf.A8Fnv ?=
ti.targets.arm.elf.M3 ?=
ti.targets.arm.elf.M4 ?=
ti.targets.arm.elf.M4F ?=

ti.targets.msp430.elf.MSP430X ?=
ti.targets.msp430.elf.MSP430X_small ?=

ti.targets.arp32.elf.ARP32 ?=
ti.targets.arp32.elf.ARP32_far ?=

gnu.targets.arm.M3 ?=
gnu.targets.arm.M4 ?=
gnu.targets.arm.M4F ?=

gnu.targets.arm.A8F ?=
gnu.targets.arm.A9F ?=
gnu.targets.arm.A15F ?=

iar.targets.msp430.MSP430X_small ?=

iar.targets.arm.M3 ?=
iar.targets.arm.M4 ?=
iar.targets.arm.M4F ?=

#
# Set XDCARGS to some of the variables above.  XDCARGS are passed
# to the XDC build engine... which will load uia.bld... which will
# extract these variables and use them to determine what to build and which
# toolchains to use.
#
# Note that not all of these variables need to be set to something valid.
# Unfortunately, since these vars are unconditionally assigned, your build line
# will be longer and more noisy than necessary (e.g., it will include CC_V5T
# assignment even if you're just building for C64P).
#
# Some background is here:
#     http://rtsc.eclipse.org/docs-tip/Command_-_xdc#Environment_Variables
#
XDCARGS= \
    ti.targets.C28_large=\"$(ti.targets.C28_large)\" \
    ti.targets.C28_float=\"$(ti.targets.C28_float)\" \
    ti.targets.C64P=\"$(ti.targets.C64P)\" \
    ti.targets.C64P_big_endian=\"$(ti.targets.C64P_big_endian)\" \
    ti.targets.C64T=\"$(ti.targets.C64T)\" \
    ti.targets.C674=\"$(ti.targets.C674)\" \
    ti.targets.arm.elf.Arm9=\"$(ti.targets.arm.elf.Arm9)\" \
    ti.targets.arm.elf.A8Fnv=\"$(ti.targets.arm.elf.A8Fnv)\" \
    ti.targets.arm.elf.M3=\"$(ti.targets.arm.elf.M3)\" \
    ti.targets.arm.elf.M4=\"$(ti.targets.arm.elf.M4)\" \
    ti.targets.arm.elf.M4F=\"$(ti.targets.arm.elf.M4F)\" \
    ti.targets.elf.C64P=\"$(ti.targets.elf.C64P)\" \
    ti.targets.elf.C64P_big_endian=\"$(ti.targets.elf.C64P_big_endian)\" \
    ti.targets.elf.C64T=\"$(ti.targets.elf.C64T)\" \
    ti.targets.elf.C66=\"$(ti.targets.elf.C66)\" \
    ti.targets.elf.C66_big_endian=\"$(ti.targets.elf.C66_big_endian)\" \
    ti.targets.elf.C674=\"$(ti.targets.elf.C674)\" \
    ti.targets.msp430.elf.MSP430X=\"$(ti.targets.msp430.elf.MSP430X)\" \
    ti.targets.msp430.elf.MSP430X_small=\"$(ti.targets.msp430.elf.MSP430X_small)\" \
    ti.targets.arp32.elf.ARP32=\"$(ti.targets.arp32.elf.ARP32)\" \
    ti.targets.arp32.elf.ARP32_far=\"$(ti.targets.arp32.elf.ARP32_far)\" \
    gnu.targets.arm.M3=\"$(gnu.targets.arm.M3)\" \
    gnu.targets.arm.M4=\"$(gnu.targets.arm.M4)\" \
    gnu.targets.arm.M4F=\"$(gnu.targets.arm.M4F)\" \
    gnu.targets.arm.A8F=\"$(gnu.targets.arm.A8F)\"\
    gnu.targets.arm.A9F=\"$(gnu.targets.arm.A9F)\"\
    gnu.targets.arm.A15F=\"$(gnu.targets.arm.A15F)\"\
    iar.targets.msp430.MSP430X_small=\"$(iar.targets.msp430.MSP430X_small)\" \
    iar.targets.arm.M3=\"$(iar.targets.arm.M3)\" \
    iar.targets.arm.M4=\"$(iar.targets.arm.M4)\" \
    iar.targets.arm.M4F=\"$(iar.targets.arm.M4F)\"

#
# Set XDCPATH to contain necessary repositories.
#
XDCPATH = $(NDK_INSTALL_DIR)/packages;$(IPC_INSTALL_DIR)/packages;$(BIOS_INSTALL_DIR)/packages
export XDCPATH

#
# Set XDCOPTIONS.  Use -v for a verbose build.
#
#XDCOPTIONS=v
export XDCOPTIONS

#
# Set XDC executable command
# Note that XDCBUILDCFG points to the uia.bld file which uses
# the arguments specified by XDCARGS
#
XDC = $(XDC_INSTALL_DIR)/xdc XDCARGS="$(XDCARGS)" XDCBUILDCFG=./uia.bld

######################################################
## Shouldnt have to modify anything below this line ##
######################################################

all:
	@ echo building uia packages ...
	@ $(XDC) -Pr ./packages

clean:
	@ echo cleaning uia packages ...
	@ $(XDC) clean -Pr ./packages

install-packages:
	@ echo installing uia packages to $(DESTDIR) ...
	@ mkdir -p $(DESTDIR)/$(packageprefix)/$(packagedocdir)
	@ cp -rf $(wildcard uia_*_release_notes.html) docs/* $(DESTDIR)/$(packageprefix)/$(packagedocdir)
	@ mkdir -p $(DESTDIR)/$(packageprefix)/$(packagesdir)
	@ cp -rf packages/* $(DESTDIR)/$(packageprefix)/$(packagesdir)

install:
	@ echo installing ti/uia to $(DESTDIR) ...
	@ mkdir -p $(DESTDIR)/$(prefix)/$(docdir)
	@ cp -rf $(wildcard uia_*_release_notes.html) docs/* $(DESTDIR)/$(prefix)/$(docdir)
	@ mkdir -p $(DESTDIR)/$(prefix)/$(includedir)/ti/uia
	@ cp -rf packages/ti/uia/*.h $(DESTDIR)/$(prefix)/$(includedir)/ti/uia
