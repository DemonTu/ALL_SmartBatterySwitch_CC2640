/*
 * Copyright (c) 2015, Texas Instruments Incorporated
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
 */
/*
 *  ======== pthread_cond.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include <ti/sysbios/posix/pthread.h>
#include <ti/sysbios/posix/_pthread_error.h>

#define _PTHREAD_DEBUG 1

/*
 *
 */
typedef struct CondElem {
    Queue_Elem       qElem;
    Semaphore_Struct sem;
} CondElem;


/*
 *  ======== pthread_cond_Obj ========
 */
typedef struct pthread_Cond_Obj {
    Queue_Struct     waitList;
} pthread_cond_Obj;

static int condWait(pthread_cond_Obj *cond, pthread_mutex_t *mutex,
        UInt32 timeout);

/*
 *************************************************************************
 *                      pthread_condattr
 *************************************************************************
 */
/*
 *  ======== pthread_condattr_destroy ========
 */
int pthread_condattr_destroy(pthread_condattr_t *attr)
{
    return (0);
}

/*
 *  ======== pthread_condattr_getpshared ========
 */
int pthread_condattr_getpshared(const pthread_condattr_t *attr, int *pshared)
{
    *pshared = attr->pshared;
    return (0);
}

/*
 *  ======== pthread_condattr_init ========
 */
int pthread_condattr_init(pthread_condattr_t * attr)
{
    attr->pshared = PTHREAD_PROCESS_PRIVATE;
    return (0);
}

/*
 *  ======== pthread_condattr_setpshared ========
 */
int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared)
{
    if ((pshared != PTHREAD_PROCESS_PRIVATE) &&
            (pshared != PTHREAD_PROCESS_SHARED)) {
        return (EINVAL);
    }

    attr->pshared = pshared;
    return (0);
}


/*
 *************************************************************************
 *                      pthread_cond
 *************************************************************************
 */
/*
 *  ======== pthread_cond_broadcast ========
 */
int pthread_cond_broadcast(pthread_cond_t *cond)
{
    pthread_cond_Obj  *condObj = (pthread_cond_Obj *)*cond;
    CondElem          *condElem;

    /* Calling thread is holding the mutex */

    while (!Queue_empty(Queue_handle(&(condObj->waitList)))) {
        condElem = (CondElem *)Queue_dequeue(Queue_handle(&(condObj->waitList)));

        Semaphore_post(Semaphore_handle(&condElem->sem));
    }

    return (0);
}

/*
 *  ======== pthread_cond_destroy ========
 */
int pthread_cond_destroy(pthread_cond_t *cond)
{
    pthread_cond_Obj *condObj = (pthread_cond_Obj *)*cond;

    Queue_destruct(&condObj->waitList);

    Memory_free(Task_Object_heap(), condObj, sizeof(pthread_cond_Obj));

    *cond = PTHREAD_COND_INITIALIZER;

    return (0);
}

/*
 *  ======== pthread_cond_init ========
 */
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
    pthread_cond_Obj   *condObj;
    Error_Block         eb;

    Error_init(&eb);

    *cond = PTHREAD_COND_INITIALIZER;

    // TODO: Is Task_Object_heap() ok to use?
    condObj = (pthread_cond_Obj *)Memory_alloc(Task_Object_heap(),
            sizeof(pthread_cond_Obj), 0, &eb);

    if (condObj == NULL) {
        return (ENOMEM);
    }

    Queue_construct(&(condObj->waitList), NULL);
    *cond = (pthread_cond_t)condObj;

    return (0);
}

/*
 *  ======== pthread_cond_signal ========
 */
int pthread_cond_signal(pthread_cond_t *cond)
{
    pthread_cond_Obj  *condObj = (pthread_cond_Obj *)*cond;
    CondElem          *condElem;

    /* The calling thread is holding the mutex */
    condElem = (CondElem *)Queue_dequeue(Queue_handle(&(condObj->waitList)));

    Semaphore_post(Semaphore_handle(&condElem->sem));

    return (0);
}

/*
 *  ======== pthread_cond_timedwait ========
 */
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
        const struct timespec *abstime)
{
    pthread_cond_Obj  *condObj = (pthread_cond_Obj *)*cond;
    UInt32             timeout;
    long               usecs;

    if ((abstime->tv_nsec < 0) || (1000000000 < abstime->tv_nsec)) {
        return (EINVAL);
    }

    usecs = abstime->tv_sec * 1000000 + abstime->tv_nsec / 1000;
    timeout = usecs / Clock_tickPeriod;

    return (condWait(condObj, mutex, timeout));
}

/*
 *  ======== pthread_cond_wait ========
 */
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    pthread_cond_Obj *condObj = (pthread_cond_Obj *)*cond;

    return (condWait(condObj, mutex, BIOS_WAIT_FOREVER));
}

/*
 *  ======== pthread_cond_wait ========
 */
static int condWait(pthread_cond_Obj *cond, pthread_mutex_t *mutex,
        UInt32 timeout)
{
    CondElem          condElem;
    Semaphore_Params  semParams;
    int               ret = 0;

    /*
     *  The calling thread is holding mutex so we don't need to call
     *  Task_disable() to protect the condition variable's waitList.
     */

    Queue_elemClear(&(condElem.qElem));

    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    Semaphore_construct(&(condElem.sem), 0, &semParams);

    Queue_enqueue(Queue_handle(&(cond->waitList)), (Queue_Elem *)&condElem);

    pthread_mutex_unlock(mutex);

    if (!Semaphore_pend(Semaphore_handle(&condElem.sem), timeout)) {
        ret = ETIMEDOUT;
    }

    pthread_mutex_lock(mutex);

    return (ret);
}
