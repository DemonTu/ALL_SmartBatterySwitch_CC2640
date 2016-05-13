
XOPTS = -I"C:/ti/xdctools_3_31_01_33_core/packages/" -Dxdc_target_types__=C:/ti/tirtos_simplelink_2_13_00_06/products/bios_6_42_00_08/packages/iar/targets/arm/std.h -Dxdc_target_name__=M3

vpath % C:/ti/tirtos_simplelink_2_13_00_06/products/bios_6_42_00_08/packages/ti/sysbios/
vpath %.c C:/ti/xdctools_3_31_01_33_core/packages/

XDC_ROOT  = C:/ti/xdctools_3_31_01_33_core/packages/

BIOS_ROOT = C:/ti/tirtos_simplelink_2_13_00_06/products/bios_6_42_00_08/packages/ti/sysbios/

BIOS_DEFS =  -Dti_sysbios_BIOS_swiEnabled__D=TRUE -Dti_sysbios_BIOS_taskEnabled__D=TRUE -Dti_sysbios_BIOS_clockEnabled__D=TRUE -Dti_sysbios_BIOS_runtimeCreatesEnabled__D=TRUE -Dti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS -Dti_sysbios_knl_Swi_DISABLE_ALL_HOOKS -Dti_sysbios_knl_Task_DISABLE_ALL_HOOKS -Dti_sysbios_BIOS_smpEnabled__D=FALSE -Dti_sysbios_Build_useHwiMacros -Dti_sysbios_knl_Swi_numPriorities__D=6 -Dti_sysbios_knl_Task_deleteTerminatedTasks__D=FALSE -Dti_sysbios_knl_Task_numPriorities__D=6 -Dti_sysbios_knl_Task_checkStackFlag__D=FALSE -Dti_sysbios_knl_Clock_TICK_SOURCE=ti_sysbios_knl_Clock_TickSource_TIMER -Dti_sysbios_knl_Clock_TICK_MODE=ti_sysbios_knl_Clock_TickMode_DYNAMIC -Dti_sysbios_hal_Core_delegate_getId=ti_sysbios_hal_CoreNull_getId__E -Dti_sysbios_hal_Core_delegate_interruptCore=ti_sysbios_hal_CoreNull_interruptCore__E -Dti_sysbios_hal_Core_delegate_lock=ti_sysbios_hal_CoreNull_lock__E -Dti_sysbios_hal_Core_delegate_unlock=ti_sysbios_hal_CoreNull_unlock__E -Dti_sysbios_hal_Core_numCores__D=1 -Dti_sysbios_hal_CoreNull_numCores__D=1 -Dti_sysbios_utils_Load_taskEnabled__D=TRUE -Dti_sysbios_utils_Load_swiEnabled__D=FALSE -Dti_sysbios_utils_Load_hwiEnabled__D=FALSE -Dti_sysbios_family_arm_m3_Hwi_dispatcherSwiSupport__D=TRUE -Dti_sysbios_family_arm_m3_Hwi_dispatcherTaskSupport__D=TRUE -Dti_sysbios_family_arm_m3_Hwi_dispatcherAutoNestingSupport__D=TRUE -Dti_sysbios_family_arm_m3_Hwi_dispatcherIrpTrackingSupport__D=TRUE -Dti_sysbios_knl_Semaphore_supportsEvents__D=FALSE -Dti_sysbios_knl_Semaphore_supportsPriority__D=FALSE -Dxdc_runtime_Assert_DISABLE_ALL -Dxdc_runtime_Log_DISABLE_ALL

BIOS_INC  = -I"C:/ti/tirtos_simplelink_2_13_00_06/products/bios_6_42_00_08/packages/"

TARGET_INC = -I"C:/ti/tirtos_simplelink_2_13_00_06/products/bios_6_42_00_08/packages/"

INCS = $(BIOS_INC) $(TARGET_INC)   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../Source/Application/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Projects/ble/Include/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Projects/ble/ICall/Include/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Projects/ble/Profiles/Roles/CC26xx/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Projects/ble/Profiles/Roles/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Projects/ble/Profiles/Keys/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Projects/ble/Profiles/DevInfo/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Projects/ble/Profiles/SensorProfile/cc26xx/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Projects/ble/Profiles/OAD/cc26xxST/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Projects/ble/common/cc26xx/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Components/applib/heap/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Components/ble/hci/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Components/ble/controller/CC26xx/include/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Components/ble/host/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Components/hal/target/CC2650TIRTOS/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Components/hal/target/_common/cc26xx/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Components/hal/include/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Components/osal/include/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Components/services/sdata/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Components/services/saddr/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Components/icall/include/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Components/icall/ports/tirtos/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../../../../../Components/ble/include/   -IC:/ti/tirtos_simplelink_2_13_00_06/products/cc26xxware_2_21_01_15600/   -IC:/ti/tirtos_simplelink_2_13_00_06/packages/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../Source/Application/Board_patch/interfaces/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../Source/Application/Board_patch/devices/   -IC:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SensorTag/CC26xx/IAR/Application/CC2650/../../../Source/Application/Board_patch/CC26XXST_0120/  

CCOPTS    = --aeabi --cpu=Cortex-M3 --diag_suppress=Pa050,Go005 --endian=little -e --thumb --silent  --mfc -Ohz  -Dti_sysbios_family_arm_cc26xx_Boot_driverlibVersion=2 -Dti_sysbios_family_arm_m3_Hwi_enableException__D=TRUE -Dti_sysbios_family_arm_m3_Hwi_disablePriority__D=32U -Dti_sysbios_family_arm_m3_Hwi_numSparseInterrupts__D=0U  --dlib_config C:/PROGRA~2/IARSYS~1/EMBEDD~1.2/arm/inc/c/DLib_Config_Normal.h

CC = LC_ALL=C  C:/PROGRA~2/IARSYS~1/EMBEDD~1.2/arm/bin/iccarm $(CCOPTS)

ASM = LC_ALL=C  C:/PROGRA~2/IARSYS~1/EMBEDD~1.2/arm/bin/iasmarm --cpu Cortex-M3 --endian little

AR = LC_ALL=C  C:/PROGRA~2/IARSYS~1/EMBEDD~1.2/arm/bin/iarchive 

DEL = cmd.exe /q /c del /f
CP = cmd.exe /q /c copy /Y

define RM
	$(if $(wildcard $1),$(DEL) $1,:)
endef

define ASSEMBLE
	@echo asmrm3 $< ...
	@$(ASM) $(BIOS_DEFS) $(XOPTS) $(INCS) $<
endef

all: sysbios.arm3

m3_Hwi_asm_iar.obj: family/arm/m3/Hwi_asm_iar.sv7M makefile
	@-$(call RM, $@)
	$(ASSEMBLE) -S -o m3_Hwi_asm_iar.obj
m3_Hwi_asm_switch_iar.obj: family/arm/m3/Hwi_asm_switch_iar.sv7M makefile
	@-$(call RM, $@)
	$(ASSEMBLE) -S -o m3_Hwi_asm_switch_iar.obj
m3_IntrinsicsSupport_asm_iar.obj: family/arm/m3/IntrinsicsSupport_asm_iar.sv7M makefile
	@-$(call RM, $@)
	$(ASSEMBLE) -S -o m3_IntrinsicsSupport_asm_iar.obj
m3_TaskSupport_asm_iar.obj: family/arm/m3/TaskSupport_asm_iar.sv7M makefile
	@-$(call RM, $@)
	$(ASSEMBLE) -S -o m3_TaskSupport_asm_iar.obj

BIOS.o: BIOS.c knl/Clock.c knl/Idle.c knl/Intrinsics.c knl/Queue.c knl/Semaphore.c knl/Swi.c knl/Swi_andn.c knl/Task.c hal/Hwi.c hal/Hwi_stack.c hal/Hwi_startup.c family/arm/m3/Hwi.c family/arm/m3/TaskSupport.c family/arm/cc26xx/Boot.c family/arm/cc26xx/Power.c family/arm/cc26xx/Power_standbyPolicy.c family/arm/cc26xx/Power_calibrateRCOSC.c family/arm/cc26xx/TimerGPT.c family/arm/cc26xx/Timer.c gates/GateHwi.c gates/GateMutex.c xdc/runtime/xdc_noinit.c xdc/runtime/Assert.c xdc/runtime/Core-mem.c xdc/runtime/Core-smem.c xdc/runtime/Core-label.c xdc/runtime/Core-params.c xdc/runtime/Diags.c xdc/runtime/Error.c xdc/runtime/Gate.c xdc/runtime/Log.c xdc/runtime/Memory.c xdc/runtime/HeapMin.c xdc/runtime/Registry.c xdc/runtime/Startup.c xdc/runtime/System.c xdc/runtime/SysCallback.c xdc/runtime/Text.c makefile
	@-$(call RM, BIOS.o)
	@echo clrm3 $< ...
	@$(CC) -o BIOS.o $(BIOS_DEFS) $(XOPTS) $(INCS) \
	$(BIOS_ROOT)BIOS.c \
	$(BIOS_ROOT)knl/Clock.c \
	$(BIOS_ROOT)knl/Idle.c \
	$(BIOS_ROOT)knl/Intrinsics.c \
	$(BIOS_ROOT)knl/Queue.c \
	$(BIOS_ROOT)knl/Semaphore.c \
	$(BIOS_ROOT)knl/Swi.c \
	$(BIOS_ROOT)knl/Swi_andn.c \
	$(BIOS_ROOT)knl/Task.c \
	$(BIOS_ROOT)hal/Hwi.c \
	$(BIOS_ROOT)hal/Hwi_stack.c \
	$(BIOS_ROOT)hal/Hwi_startup.c \
	$(BIOS_ROOT)family/arm/m3/Hwi.c \
	$(BIOS_ROOT)family/arm/m3/TaskSupport.c \
	$(BIOS_ROOT)family/arm/cc26xx/Boot.c \
	$(BIOS_ROOT)family/arm/cc26xx/Power.c \
	$(BIOS_ROOT)family/arm/cc26xx/Power_standbyPolicy.c \
	$(BIOS_ROOT)family/arm/cc26xx/Power_calibrateRCOSC.c \
	$(BIOS_ROOT)family/arm/cc26xx/TimerGPT.c \
	$(BIOS_ROOT)family/arm/cc26xx/Timer.c \
	$(BIOS_ROOT)gates/GateHwi.c \
	$(BIOS_ROOT)gates/GateMutex.c \
	$(XDC_ROOT)xdc/runtime/xdc_noinit.c \
	$(XDC_ROOT)xdc/runtime/Assert.c \
	$(XDC_ROOT)xdc/runtime/Core-mem.c \
	$(XDC_ROOT)xdc/runtime/Core-smem.c \
	$(XDC_ROOT)xdc/runtime/Core-label.c \
	$(XDC_ROOT)xdc/runtime/Core-params.c \
	$(XDC_ROOT)xdc/runtime/Diags.c \
	$(XDC_ROOT)xdc/runtime/Error.c \
	$(XDC_ROOT)xdc/runtime/Gate.c \
	$(XDC_ROOT)xdc/runtime/Log.c \
	$(XDC_ROOT)xdc/runtime/Memory.c \
	$(XDC_ROOT)xdc/runtime/HeapMin.c \
	$(XDC_ROOT)xdc/runtime/Registry.c \
	$(XDC_ROOT)xdc/runtime/Startup.c \
	$(XDC_ROOT)xdc/runtime/System.c \
	$(XDC_ROOT)xdc/runtime/SysCallback.c \
	$(XDC_ROOT)xdc/runtime/Text.c \

sysbios.arm3: BIOS.o m3_Hwi_asm_iar.obj m3_Hwi_asm_switch_iar.obj m3_IntrinsicsSupport_asm_iar.obj m3_TaskSupport_asm_iar.obj
	@echo arrm3 $^ ...
	@-$(call RM, $@)
	@$(AR) $@ $^


clean:
	@-$(call RM, sysbios.arm3)
	@-$(call RM, BIOS.o)
	@-$(call RM, m3_Hwi_asm_iar.obj)
	@-$(call RM, m3_Hwi_asm_switch_iar.obj)
	@-$(call RM, m3_IntrinsicsSupport_asm_iar.obj)
	@-$(call RM, m3_TaskSupport_asm_iar.obj)

