# invoke SourceDir generated makefile for appBLE.prm3
appBLE.prm3: .libraries,appBLE.prm3
.libraries,appBLE.prm3: package/cfg/appBLE_prm3.xdl
	$(MAKE) -f C:\work\source\ALL_SmartBatterySwitch_CC2640\simplelink\ble_cc26xx_2_01_01_44627\Projects\ble\HostTest\CC26xx\IAR\Config/src/makefile.libs

clean::
	$(MAKE) -f C:\work\source\ALL_SmartBatterySwitch_CC2640\simplelink\ble_cc26xx_2_01_01_44627\Projects\ble\HostTest\CC26xx\IAR\Config/src/makefile.libs clean

