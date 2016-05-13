#
#  ======== tirtos.mak ========
#

#
# Set location of various cgtools
# These variables can be set here or on the command line.
#
CCS_BUILD ?= true
DEFAULT_INSTALLATION_DIR  ?= c:/ti
ti.targets.arm.elf.M3     ?= $(DEFAULT_INSTALLATION_DIR)/ccsv6/tools/compiler/ti-cgt-arm_5.2.2
ti.targets.arm.elf.M4     ?= $(DEFAULT_INSTALLATION_DIR)/ccsv6/tools/compiler/ti-cgt-arm_5.2.2

#
# Enable TI-RTOS to build for IAR.
# Set IAR_BUILD to true and modify path to toolchain
#
IAR_BUILD ?= false
IAR_COMPILER_INSTALLATION_DIR ?= C:/Program Files (x86)/IAR Systems/Embedded Workbench 6.5
iar.targets.arm.M3            ?= $(IAR_COMPILER_INSTALLATION_DIR)/arm
iar.targets.arm.M4            ?= $(IAR_COMPILER_INSTALLATION_DIR)/arm

#
# Enable TI-RTOS to build for GCC.
# Set GCC_BUILD to true and modify path to toolchain
#
GCC_BUILD ?= false
GCC_INSTALLATION_DIR ?= $(DEFAULT_INSTALLATION_DIR)/ccsv6/tools/compiler/gcc-arm-none-eabi-4_8-2014q3
gnu.targets.arm.M3   ?= $(GCC_INSTALLATION_DIR)
gnu.targets.arm.M4   ?= $(GCC_INSTALLATION_DIR)

#
# TI-RTOS and XDCTools settings
#
XDCTOOLS_INSTALLATION_DIR ?= $(DEFAULT_INSTALLATION_DIR)/xdctools_3_31_01_33_core
export XDCTOOLS_JAVA_HOME ?= $(DEFAULT_INSTALLATION_DIR)/ccsv6/eclipse/jre

TIRTOS_INSTALLATION_VER   ?= tirtos_simplelink_2_13_00_06
TIRTOS_EXAMPLES_DIR       ?= $(TIRTOS_INSTALLATION_VER)_examples
TIRTOS_INSTALLATION_DIR   ?= $(DEFAULT_INSTALLATION_DIR)/$(TIRTOS_INSTALLATION_VER)
BIOS_INSTALLATION_DIR     ?= $(TIRTOS_INSTALLATION_DIR)/products/bios_6_42_00_08
UIA_INSTALLATION_DIR      ?= $(TIRTOS_INSTALLATION_DIR)/products/uia_2_00_02_39
NDK_INSTALLATION_DIR      ?= $(TIRTOS_INSTALLATION_DIR)/products/ndk_2_24_02_31
CCWARE_INSTALLATION_DIR   ?= $(TIRTOS_INSTALLATION_DIR)/products/CC3200_driverlib_1.1.0
CC26XXWARE_INSTALLATION_DIR   ?= $(TIRTOS_INSTALLATION_DIR)/products/cc26xxware_2_21_01_15600
CC13XXWARE_INSTALLATION_DIR   ?= $(TIRTOS_INSTALLATION_DIR)/products/cc13xxware_2_00_01_15600

#
# Set XDCARGS to some of the variables above.  XDCARGS are passed
# to the XDC build engine... which will load tirtos.bld... which will
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
    profile='release' \
    CCWareDir='$(CCWARE_INSTALLATION_DIR)' \
    CC26xxWareDir='$(CC26XXWARE_INSTALLATION_DIR)' \
    CC13xxWareDir='$(CC13XXWARE_INSTALLATION_DIR)'

ifeq ("$(CCS_BUILD)", "true")
    XDCARGS += \
    ti.targets.arm.elf.M3='$(ti.targets.arm.elf.M3)' \
	ti.targets.arm.elf.M4='$(ti.targets.arm.elf.M4)'
endif
ifeq ("$(IAR_BUILD)", "true")
	XDCARGS += \
    iar.targets.arm.M3='$(iar.targets.arm.M3)' \
	iar.targets.arm.M4='$(iar.targets.arm.M4)'
endif
ifeq ("$(GCC_BUILD)", "true")
	XDCARGS += \
    gnu.targets.arm.M3='$(gnu.targets.arm.M3)' \
	gnu.targets.arm.M4='$(gnu.targets.arm.M4)'
endif

export XDCARGS
#
# Set XDCPATH to contain necessary repositories.
#
XDCPATH = $(TIRTOS_INSTALLATION_DIR)/packages;$(UIA_INSTALLATION_DIR)/packages;$(BIOS_INSTALLATION_DIR)/packages;
export XDCPATH

#
# Set XDCOPTIONS.  Use -v for a verbose build.
#
#XDCOPTIONS=v
export XDCOPTIONS

#
# Set XDC executable command
# Note that XDCBUILDCFG points to the tirtos.bld file which uses
# the arguments specified by XDCARGS
#
XDC = $(XDCTOOLS_INSTALLATION_DIR)/xdc XDCARGS="$(XDCARGS)" XDCBUILDCFG=./tirtos.bld
XS =  $(XDCTOOLS_INSTALLATION_DIR)/xs

#######################################################
## Shouldn't have to modify anything below this line ##
#######################################################

help:
	@ echo Makefile to build components within TI-RTOS
	@ echo   goal              description
	@ echo  -----------------------------------------------------------------------------
	@ echo   all               Builds SYS/BIOS, UIA, and TI-RTOS drivers
	@ echo   drivers           Builds TI-RTOS drivers and other components in /packages
	@ echo   bios              Builds SYS/BIOS
	@ echo   uia               Builds UIA
	@ echo   clean             Cleans SYS/BIOS, UIA, and TI-RTOS drivers
	@ echo   clean-drivers     Cleans TI-RTOS drivers and other components in /packages
	@ echo   clean-bios        Cleans SYS/BIOS
	@ echo   clean-uia         Cleans UIA
	@ echo   examplesgen       Generates TI-RTOS examples and makefiles
	@ echo   help              Displays this description

all: bios uia drivers

clean: clean-uia clean-drivers clean-bios

drivers:
	@ echo building tirtos packages ...
	@ $(XDC) -Pr ./packages

clean-drivers:
	@ echo cleaning tirtos packages ...
	@ $(XDC) clean -Pr ./packages

bios:
	@ echo building bios ...
	@ $(XDCTOOLS_INSTALLATION_DIR)/gmake -f $(BIOS_INSTALLATION_DIR)/bios.mak \
	  XDC_INSTALL_DIR=$(XDCTOOLS_INSTALLATION_DIR) \
	  BIOS_SMPENABLED=0 \
	  $(XDCARGS) -C $(BIOS_INSTALLATION_DIR)

clean-bios:
	@ echo cleaning bios ...
	@ $(XDCTOOLS_INSTALLATION_DIR)/gmake -f $(BIOS_INSTALLATION_DIR)/bios.mak \
	  XDC_INSTALL_DIR=$(XDCTOOLS_INSTALLATION_DIR) \
	  BIOS_SMPENABLED=0 \
	  -C $(BIOS_INSTALLATION_DIR) clean

uia:
	@ echo building uia ...
	@ $(XDCTOOLS_INSTALLATION_DIR)/gmake -f $(UIA_INSTALLATION_DIR)/uia.mak \
	  XDC_INSTALL_DIR=$(XDCTOOLS_INSTALLATION_DIR) \
	  BIOS_INSTALL_DIR=$(BIOS_INSTALLATION_DIR) \
	  $(XDCARGS) -C $(UIA_INSTALLATION_DIR)

clean-uia:
	@ echo cleaning uia ...
	@ $(XDCTOOLS_INSTALLATION_DIR)/gmake -f $(UIA_INSTALLATION_DIR)/uia.mak \
	  XDC_INSTALL_DIR=$(XDCTOOLS_INSTALLATION_DIR) \
	  BIOS_INSTALL_DIR=$(BIOS_INSTALLATION_DIR) \
	  -C $(UIA_INSTALLATION_DIR) clean

examplesgen:
ifneq ("$(DEST)","")
	@ echo generating examples in $(DEST) ...
ifeq ("$(CCS_BUILD)", "true")
	@ $(XS) examples.examplesgen \
		--tirtos "$(TIRTOS_INSTALLATION_DIR)" \
		--productGroup "SimpleLink" \
		--toolchain TI \
		--output "$(DEST)/$(TIRTOS_EXAMPLES_DIR)" \
		--xdctools="$(XDCTOOLS_INSTALLATION_DIR)" \
		--bios="$(BIOS_INSTALLATION_DIR)" \
		--uia="$(UIA_INSTALLATION_DIR)" \
		--ccware="$(CCWARE_INSTALLATION_DIR)" \
		--cc13xxware="$(CC13XXWARE_INSTALLATION_DIR)" \
		--cc26xxware="$(CC26XXWARE_INSTALLATION_DIR)" \
		--toolChaindir="$(ti.targets.arm.elf.M4)"
endif
ifeq ("$(IAR_BUILD)", "true")
	@ $(XS) examples.examplesgen \
		--tirtos "$(TIRTOS_INSTALLATION_DIR)" \
		--productGroup "SimpleLink" \
		--toolchain IAR \
		--output "$(DEST)/$(TIRTOS_EXAMPLES_DIR)" \
		--xdctools="$(XDCTOOLS_INSTALLATION_DIR)" \
		--bios="$(BIOS_INSTALLATION_DIR)" \
		--uia="$(UIA_INSTALLATION_DIR)" \
		--ccware="$(CCWARE_INSTALLATION_DIR)" \
		--cc13xxware="$(CC13XXWARE_INSTALLATION_DIR)" \
		--cc26xxware="$(CC26XXWARE_INSTALLATION_DIR)" \
		--toolChaindir="$(iar.targets.arm.M4)"
endif
ifeq ("$(GCC_BUILD)", "true")
	@ $(XS) examples.examplesgen \
		--tirtos "$(TIRTOS_INSTALLATION_DIR)" \
		--productGroup "SimpleLink" \
		--toolchain GNU \
		--output "$(DEST)/$(TIRTOS_EXAMPLES_DIR)" \
		--xdctools="$(XDCTOOLS_INSTALLATION_DIR)" \
		--bios="$(BIOS_INSTALLATION_DIR)" \
		--uia="$(UIA_INSTALLATION_DIR)" \
		--ccware="$(CCWARE_INSTALLATION_DIR)" \
		--cc13xxware="$(CC13XXWARE_INSTALLATION_DIR)" \
		--cc26xxware="$(CC26XXWARE_INSTALLATION_DIR)" \
		--toolChaindir="$(gnu.targets.arm.M4)"
endif
	@ echo ***********************************************************
	@ echo Please refer to "Examples for TI-RTOS" section in the TI-RTOS
	@ echo "Getting Started Guide" for details on how to build and load the examples
	@ echo into IAR WorkBench and CCS.
else
	@ echo Specify destination path like this: DEST="YOURPATH". Use UNIX style path "C:/examples"
endif
