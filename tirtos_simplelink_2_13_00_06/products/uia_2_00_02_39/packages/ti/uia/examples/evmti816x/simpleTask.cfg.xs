/*
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * */
/*
 *  ======== commonApp.cfg.xs ========
 */

/* ================ XDC Configuration ================ */
/*
 *  The SysStd System provider is a good one to use for debugging
 *  but does not have the best performance. Use xdc.runtime.SysMin
 *  for better performance.
 */

var System   = xdc.useModule('xdc.runtime.System');
var SysMin   = xdc.useModule('xdc.runtime.SysMin');
System.SupportProxy = SysMin;

var Log = xdc.useModule('xdc.runtime.Log');

/* ================ BIOS Configuration ================ */
var BIOS      = xdc.useModule('ti.sysbios.BIOS');
var Clock     = xdc.useModule('ti.sysbios.knl.Clock');
var Task      = xdc.useModule('ti.sysbios.knl.Task');
var Semaphore = xdc.useModule('ti.sysbios.knl.Semaphore');
var Load      = xdc.useModule('ti.sysbios.utils.Load');

var clockParams = new Clock.Params();
clockParams.period = 30;
clockParams.startFlag = true;
var myClock  = Clock.create("&clockFxn", 30, clockParams);
myClock.instance.name = "Adaptor Clock";

/*
 * Create Semaphore for Task thread to pend on
 */
Program.global.sem = Semaphore.create(0);

/*
 * Create Task load thread
 */
var taskParams = new Task.Params();
taskParams.priority = 2;
var myTask = Task.create('&task', taskParams);
myTask.instance.name = "App Task";

/* ================ IPC Configuration ================ */
var Ipc               = xdc.useModule('ti.sdo.ipc.Ipc');
var MultiProc         = xdc.useModule('ti.sdo.utils.MultiProc');
var SharedRegion      = xdc.useModule('ti.sdo.ipc.SharedRegion');
var Syslink           = xdc.useModule ('ti.syslink.ipc.rtos.Syslink');

xdc.loadPackage ('ti.syslink.ipc.rtos');
xdc.loadPackage ('ti.syslink.utils.rtos.bios6');

/* Shared Memory Map */
var SHAREDMEM                       = 0x8E000000;
var SHAREDMEMSIZE                   = 0x01000000;

/* Ipc */
Ipc.hostProcId = MultiProc.getIdMeta("HOST");

/*
 *  IPC constants that all three programs use.
 */
Ipc.procSync = Ipc.ProcSync_PAIR;
for (var i = 0; i < MultiProc.numProcessors; i++) {
    Ipc.setEntryMeta({ remoteProcId: i,});
}

/*
 *  Need to define the shared region. The IPC modules use this
 *  to make portable pointers. All processors need to add this
 *  call with their base address of the shared memory region.
 *  If the processor cannot access the memory, do not add it.
 */
SharedRegion.setEntryMeta(0,
    {
      base:        SHAREDMEM,
      len:         SHAREDMEMSIZE,
      ownerProcId: Ipc.hostProcId,
      isValid:     true,
      name:        "internal_shared_mem",
    });
