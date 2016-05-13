#
#  Copyright (c) 2012, Texas Instruments Incorporated
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#  *  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#
#  *  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
#  *  Neither the name of Texas Instruments Incorporated nor the names of
#     its contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

#
#  ======== ti.uia.examples.evmti816x/Makefile ========
#

ifneq (clean,$(MAKECMDGOALS))
ifeq (,$(wildcard bin))
    $(shell mkdir -p bin)
endif
endif

IPC_INSTALL_DIR ?= /db/vtree/library/trees/ipc/ipc-h32/exports/ipc_1_24_03_32/packages
SYSLINK_INSTALL_DIR ?= /db/atree/library/trees/sl/sl-c35/exports/syslink_2_10_08_35/packages

SYSLINK_LIBS = $(SYSLINK_INSTALL_DIR)/ti/syslink/lib/syslink.a_release
SERVICEMGR_LIBS = ../../linux/lib/servicemgr.a
LOGGERSM_LIBS = ../../linux/lib/loggersm.a
STDLIBS = -lpthread -lrt -lm

CC = /db/toolsrc/library/tools/vendors/cs/arm/arm-2009q1-203/bin/arm-none-linux-gnueabi-gcc

CPPFLAGS = -g -O2 -Wall -I. -I../../../.. -I$(SYSLINK_INSTALL_DIR) -I$(IPC_INSTALL_DIR)

OUTPUT_OPTION = -o bin/$@

uiaDemo.srcs = 	  \
	uiaDemo.c \
	procMgrApp.c

loggerSMDump.srcs = \
	loggerSMDump.c

loader.srcs = \
	loader.c \
	procMgrApp.c

all: bin/uiaDemo.out bin/loggerSMDump.out bin/loader.out

bin/uiaDemo.out: $(patsubst %.c,%.o,$(uiaDemo.srcs))
	@echo "#"
	@echo "# Making $@ ..."
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(addprefix bin/,$^) \
            $(SERVICEMGR_LIBS) $(LOGGERSM_LIBS) $(STDLIBS) $(SYSLINK_LIBS) $(LDFLAGS)

bin/loader.out: $(patsubst %.c,%.o,$(loader.srcs))
	@echo "#"
	@echo "# Making $@ ..."
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(addprefix bin/,$^) \
            $(STDLIBS) $(SYSLINK_LIBS) $(LDFLAGS)

bin/loggerSMDump.out: $(patsubst %.c,%.o,$(loggerSMDump.srcs))
	@echo "#"
	@echo "# Making $@ ..."
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(addprefix bin/,$^) \
            $(LOGGERSM_LIBS) $(STDLIBS) $(LDFLAGS)

#
#  ======== clean ========
#
clean::
	rm -f bin/*
