Building the examples with XDC
------------------------------
All benchmarks are to be ran on the evmDA830 functional simulator.

If using the default config.bld (packages\etc\config.bld.default), make
sure that the C64P target is not commented out and run "xdc"
in this directory (e.g. packages\ti\uia\examples\benchmarks).
Build.targets = [
                    C64P,
                    //GCArmv5T,
                    C674,
                    //M3,
                ];

Refer to the User_install.pdf for details about setting up the config.bld
and build environment.

loggerCircBuf example
---------------------
Overview: Prints out some LoggerCircBuf benchmarks on the console.
          Rta and ServiceMgr are not included.
          Use evmDA830 functional simulator

How to run:
   1. Load loggerCircBuf.x674 and run

Expected output:
   You should see benchmark numbers on the console.

load example
---------------------
Overview: Can be used to display benchmark numbers in DVT

How to run:
   1. Load and run oad.x64P on core 0

load example
---------------------
Overview: Can be used to display benchmark numbers in DVT

How to run:
   1. Load and run loadMP.x64P on all cores
