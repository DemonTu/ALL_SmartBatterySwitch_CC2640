Building the Linux applications
-------------------------------
To build the Linux application, modify the makefile as needed and run make.

Building the target examples with XDC (instead of using a project template in CCS)
-------------------------------------
If using the default config.bld (packages\etc\config.bld.default), make
sure that the C674 and M3 target is not commented out and run "xdc"
in this directory (e.g. packages\ti\uia\examples\evmti816x).
Build.targets = [
                    C674,
                    M3,
                ];

Refer to the System Analyzer User Guide document for details about setting up the
config.bld and build environment.

simpleTask (LoggerCircBuf) example
----------------------------------
Overview: Each image (dsp, video and vpss) runs the simpleTask app which simply
          benchmarks how long it takes to reverse bits in a buffer.

This examples is designed to work with System Analyzer. All the events from
the DSP and M3's are routed to the Linux app uiaDemo.out. uiaDemo.out routes
the events to System Analyzer via UDP packets.

How to run
----------
   1. run the uiaDemo application on linux and specific which cores and images to
      load and run. The format is
      uiaDemo.out <MultiProc_id> <image> [<MultiProc_id> <image>] [<MultiProc_id> <image>]

      For example to load all three processors do
      % ./uiaDemo.out 0 dspLoggerCircBuf.out 1 videoLoggerCircBuf.out 2 vpssLoggerCircBuf.out

   2. Run System Analyzer. Use the IP address obtain by Linux on the Arm.

   3. To quit the test, hit any character.

simpleTask (LoggerSM) example
----------------------------------
Overview: Each image (dsp, video and vpss) runs the simpleTask app which simply
          benchmarks how long it takes to reverse bits in a buffer.

This examples is designed to work without System Analyzer. All the events from the DSP and
M3's are routed to the Linux app loggerSMDump.out. loggerSMDump.out prints the
decoded strings to the Linux console.

LoggerSM decodes the events and places them in a shared memory buffer.
UIA ships example platform files in ti\uia\examples\platforms\evmti816x.
They contain a memory segment at 0x8f000000 where the LoggerSM buffer is placed.
Additionally, the videoM3 config file disables caching for this memory segment
for both the video and vpss M3 cores. The DSP's config file displays data cache
also. LoggerSM does not do any cache coherency calls, thus the need for
disabling the cache.

Note: by default the example decodes the events on the target
(LoggerSM.decode = true;). loggerSMDump displays the decoded events
directly to the linux console. If the targets do not decode the events
(LoggerSM.decode = false;), loggerSMDump writes the encoded events into
a binary file (specified as the last argument). This file can be decoded
by System Analyzer.

How to run
----------
   1. run the loader.out application on linux and specific which cores and images to
      load and run. The format is
      loader.out <MultiProc_id> <image> [<MultiProc_id> <image>] [<MultiProc_id> <image>]

      For example to load all three processors do
      % ./loader.out dspLoggerSM.out 1 videoLoggerSM.out 2 vpssLoggerSM.out

   2. In a separate console/telnet, run loggerSMDump.out. You need to specify
      the address of the LoggerSM buffer. You should start to see decoded
      events (or written to the binary file if "LoggerSM.decode = false").
      % ./loggerSMDump.out 0x8f000000 all outputFile.bin

   3. To quit the test, Ctrl-C the loggerSMDump and hit any character on the
      loader.out app.
