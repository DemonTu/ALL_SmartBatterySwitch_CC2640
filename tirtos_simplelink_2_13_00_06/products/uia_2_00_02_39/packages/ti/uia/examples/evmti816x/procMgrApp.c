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
 *  ======== procMgrApp.c ========
 */

/* Standard headers */
#include <ti/syslink/Std.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/OsalPrint.h>
#include <ti/syslink/utils/String.h>
#include <ti/syslink/IpcHost.h>

/* Module level headers */
#include <ti/syslink/ProcMgr.h>

#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */

#define NULL 0 //TODO why do I need this?

/** ============================================================================
 *  Globals
 *  ============================================================================
 */
#define NUM_ARGS 1
#define NUM_REMOTE_PROCS 3


/*!
 *  @brief  Handle to the ProcMgr instance used.
 */
ProcMgr_Handle ProcMgrApp_handle[NUM_REMOTE_PROCS];

/*!
 *  @brief  File ID of the file loaded.
 */
UInt32 ProcMgrApp_fileId[NUM_REMOTE_PROCS];


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/*!
 *  @brief  Function to execute the startup for ProcMgrApp sample application
 */
Int
ProcMgrApp_startup (UInt16 procId, String filePath)
{
    Int                          status = 0;
    ProcMgr_AttachParams         attachParams;
    ProcMgr_StartParams          startParams;
    ProcMgr_State                state;
    String                       args [NUM_ARGS];


    status = ProcMgr_open (&ProcMgrApp_handle[procId], procId);
    Osal_printf ("ProcMgr_open status [0x%x], procId = %d\n", status, procId);
    if (status >= 0) {
        ProcMgr_getAttachParams (NULL, &attachParams);
        /* Default params will be used if NULL is passed. */

        status = ProcMgr_attach (ProcMgrApp_handle[procId], &attachParams);
        if (status < 0) {
            Osal_printf ("ProcMgr_attach failed [0x%x]\n", status);
        }
        else {
            Osal_printf ("ProcMgr_attach status: [0x%x]\n", status);
            state = ProcMgr_getState (ProcMgrApp_handle[procId]);
            Osal_printf ("After attach: ProcMgr_getState\n"
                         "    state [0x%x]\n",
                         state);

            /* Send filePath as the args to the slave to demonstrate how args
             * are used.
             */
            args [0] = filePath;
            Osal_printf ("ProcMgr_load %s\n", args[0]);
            status = ProcMgr_load (ProcMgrApp_handle[procId],
                                   filePath,
                                   NUM_ARGS,
                                   args,
                                   NULL,
                                   &ProcMgrApp_fileId[procId]);
            if (status < 0) {
                Osal_printf ("Error in ProcMgr_load [0x%x]\n", status);
            }
            else {
                Osal_printf ("ProcMgr_load status: [0x%x]\n", status);
                status = Ipc_control (procId,
                                      Ipc_CONTROLCMD_LOADCALLBACK,
                                      NULL);
                if (status < 0) {
                    Osal_printf ("Error in Ipc_control "
                                 "Ipc_CONTROLCMD_LOADCALLBACK [0x%x]\n",
                                 status);
                }
                else {
                    state = ProcMgr_getState (ProcMgrApp_handle[procId]);
                    Osal_printf ("After load: ProcMgr_getState\n"
                                 "    state [0x%x]\n",
                                 state);
                }
            }

            if (status >= 0) {
                ProcMgr_getStartParams (ProcMgrApp_handle[procId], &startParams);
                status = ProcMgr_start (ProcMgrApp_handle[procId], &startParams);
                if (status < 0) {
                    Osal_printf ("ProcMgr_start failed [0x%x]\n", status);
                }
                else {
                    Osal_printf ("ProcMgr_start passed [0x%x]\n", status);
                    status = Ipc_control (procId,
                                          Ipc_CONTROLCMD_STARTCALLBACK,
                                          NULL);
                    if (status < 0) {
                        Osal_printf ("Error in Ipc_control "
                                     "Ipc_CONTROLCMD_STARTCALLBACK[0x%x]\n",
                                     status);
                    }
                    else {
                        state = ProcMgr_getState (ProcMgrApp_handle[procId]);
                        Osal_printf ("After start: ProcMgr_getState\n"
                                     "    state [0x%x]\n",
                                     state);
                    }
                }
            }
        }
    }

    return 0;
}

/*!
 *  @brief  Function to execute the shutdown for ProcMgrApp sample application
 */
Int
ProcMgrApp_shutdown (UInt16 procId)
{
    Int                          status = 0;
    ProcMgr_State                state;

    if (ProcMgrApp_handle[procId] != NULL) {
        status = Ipc_control (procId, Ipc_CONTROLCMD_STOPCALLBACK, NULL);
        Osal_printf ("Ipc_control Ipc_CONTROLCMD_STOPCALLBACK status: [0x%x]\n",
                     status);

        status = ProcMgr_stop (ProcMgrApp_handle[procId]);
        Osal_printf ("ProcMgr_stop status: [0x%x]\n", status);
        state = ProcMgr_getState (ProcMgrApp_handle[procId]);
        Osal_printf ("After stop: ProcMgr_getState\n"
                     "    state [0x%x]\n",
                     state);

        status = ProcMgr_unload (ProcMgrApp_handle[procId], ProcMgrApp_fileId[procId]) ;
        Osal_printf ("ProcMgr_unload status: [0x%x]\n", status);

        state = ProcMgr_getState (ProcMgrApp_handle[procId]);
        Osal_printf ("After unload: ProcMgr_getState\n"
                     "    state [0x%x]\n",
                     state);

        status = ProcMgr_detach (ProcMgrApp_handle[procId]);
        Osal_printf ("ProcMgr_detach status: [0x%x]\n", status);

        state = ProcMgr_getState (ProcMgrApp_handle[procId]);
        Osal_printf ("After detach: ProcMgr_getState\n"
                     "    state [0x%x]\n",
                     state);
    }

    if (ProcMgrApp_handle[procId] != NULL) {
        status = ProcMgr_close (&(ProcMgrApp_handle[procId]));
        Osal_printf ("ProcMgr_close status: [0x%x]\n", status);
    }

    return 0;
}

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
