#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /packages;/packages;/db/vtree/library/trees/zumaprod/zumaprod-g06/tirtos_simplelink_2_13_00_06/products/bios_6_42_00_08/packages
override XDCROOT = /db/rtree/library/trees/xdcprod/xdcprod-u33/product/xdccore/Linux/xdctools_3_31_01_33_core
override XDCBUILDCFG = /db/vtree/library/trees/zumaprod/zumaprod-g06/tirtos_simplelink_2_13_00_06/products/uia_2_00_02_39/uia.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = ti.targets.C28_large="" ti.targets.C28_float="" ti.targets.C64P="" ti.targets.C64P_big_endian="" ti.targets.C64T="" ti.targets.C674="" ti.targets.arm.elf.Arm9="" ti.targets.arm.elf.A8Fnv="" ti.targets.arm.elf.M3="/db/vendors/ti/arm/5.2.2/Linux" ti.targets.arm.elf.M4="/db/vendors/ti/arm/5.2.2/Linux" ti.targets.arm.elf.M4F="" ti.targets.elf.C64P="" ti.targets.elf.C64P_big_endian="" ti.targets.elf.C64T="" ti.targets.elf.C66="" ti.targets.elf.C66_big_endian="" ti.targets.elf.C674="" ti.targets.msp430.elf.MSP430X="" ti.targets.msp430.elf.MSP430X_small="" ti.targets.arp32.elf.ARP32="" ti.targets.arp32.elf.ARP32_far="" gnu.targets.arm.M3="/db/vendors/linaro/gcc-arm-none-eabi-4_8-2014q3" gnu.targets.arm.M4="/db/vendors/linaro/gcc-arm-none-eabi-4_8-2014q3" gnu.targets.arm.M4F="" gnu.targets.arm.A8F="" gnu.targets.arm.A9F="" gnu.targets.arm.A15F="" iar.targets.msp430.MSP430X_small="" iar.targets.arm.M3="/db/vendors/iar/arm/7.40.1a/Linux" iar.targets.arm.M4="/db/vendors/iar/arm/7.40.1a/Linux" iar.targets.arm.M4F=""
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /packages;/packages;/db/vtree/library/trees/zumaprod/zumaprod-g06/tirtos_simplelink_2_13_00_06/products/bios_6_42_00_08/packages;/db/rtree/library/trees/xdcprod/xdcprod-u33/product/xdccore/Linux/xdctools_3_31_01_33_core/packages;../../../..
HOSTOS = Linux
endif
