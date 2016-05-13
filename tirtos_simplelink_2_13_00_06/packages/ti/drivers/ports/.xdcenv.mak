#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = c:/ti/tirtos_simplelink_2_13_00_06/packages;/db/vtree/library/trees/zumaprod/zumaprod-g06/tirtos_simplelink_2_13_00_06/products/uia_2_00_02_39/packages;/db/vtree/library/trees/zumaprod/zumaprod-g06/tirtos_simplelink_2_13_00_06/products/bios_6_42_00_08/packages;
override XDCROOT = /db/rtree/library/trees/xdcprod/xdcprod-u33/product/xdccore/Linux/xdctools_3_31_01_33_core
override XDCBUILDCFG = /db/vtree/library/trees/zumaprod/zumaprod-g06/tirtos_simplelink_2_13_00_06/tirtos.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = profile='release' CCWareDir='/db/vtree/library/trees/zumaprod/zumaprod-g06/tirtos_simplelink_2_13_00_06/products/CC3200_driverlib_1.1.0' CC26xxWareDir='/db/vtree/library/trees/zumaprod/zumaprod-g06/tirtos_simplelink_2_13_00_06/products/cc26xxware_2_21_01_15600' CC13xxWareDir='/db/vtree/library/trees/zumaprod/zumaprod-g06/tirtos_simplelink_2_13_00_06/products/cc13xxware_2_00_01_15600' ti.targets.arm.elf.M3='/db/vendors/ti/arm/5.2.2/Linux' ti.targets.arm.elf.M4='/db/vendors/ti/arm/5.2.2/Linux' iar.targets.arm.M3='/db/vendors/iar/arm/7.40.1a/Linux' iar.targets.arm.M4='/db/vendors/iar/arm/7.40.1a/Linux' gnu.targets.arm.M3='/db/vendors/linaro/gcc-arm-none-eabi-4_8-2014q3' gnu.targets.arm.M4='/db/vendors/linaro/gcc-arm-none-eabi-4_8-2014q3'
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = c:/ti/tirtos_simplelink_2_13_00_06/packages;/db/vtree/library/trees/zumaprod/zumaprod-g06/tirtos_simplelink_2_13_00_06/products/uia_2_00_02_39/packages;/db/vtree/library/trees/zumaprod/zumaprod-g06/tirtos_simplelink_2_13_00_06/products/bios_6_42_00_08/packages;/db/rtree/library/trees/xdcprod/xdcprod-u33/product/xdccore/Linux/xdctools_3_31_01_33_core/packages;../../..
HOSTOS = Linux
endif
