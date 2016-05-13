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
 *  ======== pthread_mutex.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/IGateProvider.h>
#include <xdc/runtime/Memory.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/gates/GateMutex.h>
#include <ti/sysbios/gates/GateMutexPri.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/sysbios/posix/pthread.h>
#include <ti/sysbios/posix/_pthread.h>
#include <ti/sysbios/posix/_pthread_error.h>

#include <time.h>

#define _PTHREAD_DEBUG 1

#define DEFAULT_TYPE PTHREAD_MUTEX_RECURSIVE

/*
 *  ======== pthread_mutex_Obj ========
 */
typedef struct pthread_mutex_Obj {
#if defined(_POSIX_THREAD_PRIO_PROTECT)
    Queue_Elem                 qElem;
#endif
    Task_Handle                owner;
    int                        taskPri;
    int                        lockCnt;
    pthread_mutexattr_t        attrs;
    Semaphore_Struct           sem;
} pthread_mutex_Obj;

static int mutexAcquire(pthread_mutex_Obj *mutex, UInt32 timeout);

/*
 *************************************************************************
 *                      pthread_mutexattr
 *************************************************************************
 */
/*
 *  ======== pthread_mutexattr_destroy ========
 */
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    return (0);
}

/*
 *  ======== pthread_mutexattr_getprioceiling ========
 */
int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *attr,
        int *prioceiling)
{
#if defined(_POSIX_THREAD_PRIO_PROTECT)
    *prioceiling = attr->prioceiling;
    return (0);
#else
    return (ENOSYS);
#endif
}

/*
 *  ======== pthread_mutexattr_gettype ========
 */
int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr,
        int *type)
{
    *type = attr->type;
    return (0);
}

/*
 *  ======== pthread_mutexattr_getprotocol ========
 */
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr,
        int *protocol)
{
    *protocol = attr->protocol;
    return (0);
}

/*
 *  ======== pthread_mutexattr_init ========
 */
int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    attr->type = DEFAULT_TYPE;
    attr->protocol = PTHREAD_PRIO_INHERIT;
#if defined(_POSIX_THREAD_PRIO_PROTECT)
    attr->prioceiling = 1;
#endif
    return (0);
}

/*
 *  ======== pthread_mutexattr_setprioceiling ========
 */
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr,
        const int prioceiling)
{
#if defined(_POSIX_THREAD_PRIO_PROTECT)
    if ((prioceiling >= Task_numPriorities) ||
            ((prioceiling == 0) && (Task_getIdleTask() != NULL)) ||
            (prioceiling < -1)) {
        /* Bad priority value */
        return (EINVAL);
    }

    attr->prioceiling = prioceiling;
    return (0);
#else
    return (ENOSYS);
#endif
}

/*
 *  ======== pthread_mutexattr_setprotocol ========
 */
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol)
{
#if defined(_POSIX_THREAD_PRIO_PROTECT)
    if ((protocol != PTHREAD_PRIO_NONE) &&
            (protocol != PTHREAD_PRIO_INHERIT) &&
            (protocol != PTHREAD_PRIO_PROTECT)) {
        return (EINVAL);
    }
    attr->protocol = protocol;
    return (0);
#else
    if ((protocol != PTHREAD_PRIO_NONE) &&
            (protocol != PTHREAD_PRIO_INHERIT)) {
        return (EINVAL);
    }
    attr->protocol = protocol;
    return (0);
#endif
}

/*
 *  ======== pthread_mutexattr_settype ========
 */
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    if ((type != PTHREAD_MUTEX_NORMAL) && (type != PTHREAD_MUTEX_ERRORCHECK) &&
            (type != PTHREAD_MUTEX_RECURSIVE)) {
        return (EINVAL);
    }

    attr->type = type;
    return (0);
}

/*
 *************************************************************************
 *                      pthread_mutex
 *************************************************************************
 */


/*
 *  ======== pthread_mutex_destroy ========
 */
int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    pthread_mutex_Obj *mutexObj = (pthread_mutex_Obj *)*mutex;

    Semaphore_destruct(&mutexObj->sem);

    Memory_free(Task_Object_heap(), mutexObj, sizeof(pthread_mutex_Obj));

    *mutex = PTHREAD_MUTEX_INITIALIZER;

    return (0);
}

/*
 *  ======== pthread_mutex_getprioceiling ========
 *
 *  Retuns:
 *    0      - Success.
 *    EINVAL - Mutex protocol is not PTHREAD_PRIO_PROTECT
 *    ENOSYS - This function not supported.
 */
int pthread_mutex_getprioceiling(const pthread_mutex_t *mutex,
        int *prioceiling)
{
#if defined(_POSIX_THREAD_PRIO_PROTECT)
    pthread_mutex_Obj *mutexObj = (pthread_mutex_Obj *)*mutex;
    UInt                key;

    if (mutexObj->attrs.protocol != PTHREAD_PRIO_PROTECT) {
        return (EINVAL);
    }

    key = Task_disable();

    *prioceiling = mutexObj->attrs.prioceiling;

    Task_restore(key);

    return (0);
#else
    return (ENOSYS);
#endif
}

/*
 *  ======== pthread_mutex_init ========
 */
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
    pthread_mutex_Obj   *mutexObj;
    Semaphore_Params    semParams;
    Error_Block         eb;

    Error_init(&eb);

    *mutex = PTHREAD_MUTEX_INITIALIZER;

    // TODO: Is Task_Object_heap() ok to use?
    mutexObj = (pthread_mutex_Obj *)Memory_alloc(Task_Object_heap(),
            sizeof(pthread_mutex_Obj), 0, &eb);

    if (mutexObj == NULL) {
        return (ENOMEM);
    }

    if (attr) {
        mutexObj->attrs = *attr;
        if (attr->type == PTHREAD_MUTEX_DEFAULT) {
            mutexObj->attrs.type = PTHREAD_MUTEX_RECURSIVE;
        }
    }
    else {
        pthread_mutexattr_init(&(mutexObj->attrs));
    }

    mutexObj->owner = NULL;
    mutexObj->lockCnt = 0;

    Semaphore_Params_init(&semParams);

    /* Type of semaphore used for trylock() and timedlock(). */
    if (mutexObj->attrs.protocol == PTHREAD_PRIO_INHERIT) {
        semParams.mode = Semaphore_Mode_BINARY_PRIORITY;
    }
    else {
        semParams.mode = Semaphore_Mode_BINARY;
    }

    Semaphore_construct(&(mutexObj->sem), 1, &semParams);

    *mutex = (pthread_mutex_t)mutexObj;

    return (0);
}

/*
 *  ======== pthread_mutex_lock ========
 *
 *  Retuns:
 *    0      - Lock was successfully acquired.
 *    EINVAL - Mutex protocol is PTHREAD_PRIO_PROTECT and mutex prioceiling
 *             is lower than calling thread's priority
 *    EDEADLK - Mutex type is PTHREAD_MUTEX_ERRORCHECK and calling thread
 *              already owns the mutex.
 */
int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    pthread_mutex_Obj *mutexObj = (pthread_mutex_Obj *)*mutex;

    return (mutexAcquire(mutexObj, BIOS_WAIT_FOREVER));
}

/*
 *  ======== pthread_mutex_setprioceiling ========
 *
 *  Retuns:
 *    0      - Success.
 *    EINVAL - Mutex protocol is not PTHREAD_PRIO_PROTECT
 *           - prioceiling is out of range.
 *    ENOSYS - This function not supported.
 */
int pthread_mutex_setprioceiling(pthread_mutex_t *mutex, int prioceiling,
        int *oldceiling)
{
#if defined(_POSIX_THREAD_PRIO_PROTECT)
    pthread_mutex_Obj *mutexObj = (pthread_mutex_Obj *)*mutex;
    int                ret;
    int                priority;

    if (mutexObj->attrs.protocol != PTHREAD_PRIO_PROTECT) {
        return (EINVAL);
    }

    if ((prioceiling >= Task_numPriorities) ||
            ((prioceiling == 0) && (Task_getIdleTask() != NULL)) ||
            (prioceiling < -1)) {
        /* Bad priority value */
        return (EINVAL);
    }

    /*
     *  Lock the mutex (may block), then change the mutex
     *  priority ceiling and unlock the mutex.  Don't need to
     *  adhere to the PTHREAD_PRIO_PROTECT protocol when locking
     *  the mutex.  If we already own the mutex, check to see if
     *  this thread's priority needs to be bumped up.
     */
    if ((ret = pthread_mutex_lock(mutex)) == 0) {
        *oldceiling = mutexObj->attrs.prioceiling;
        mutexObj->attrs.prioceiling = prioceiling;

        if (mutexObj->lockCnt > 1) {
            /*
             *  Set this thread's priority to the new ceiling, if
             *  higher than the thread's priority
             */
            priority = Task_getPri(Task_self());
            if (priority < prioceiling) {
                Task_setPri(Task_self(), prioceiling);
            }
        }

        pthread_mutex_unlock(mutex);
    }

    return (ret);
#else
    /* PTHREAD_PRIO_PROTECT not supported for mutexes */
    return (ENOSYS);
#endif
}

/*
 *  ======== pthread_mutex_timedlock ========
 *
 *  Retuns:
 *    0      - Lock was successfully acquired.
 *    EINVAL - Mutex protocol is PTHREAD_PRIO_PROTECT and mutex prioceiling
 *             is lower than calling thread's priority
 *           - Bad nanoseconds value: < 0 or > 1000000000
 *    ETIMEDOUT - timeout expired before mutex could be acquired
 *    EDEADLK - Mutex type is PTHREAD_MUTEX_ERRORCHECK and calling thread
 *              already owns the mutex.
 */
int pthread_mutex_timedlock(pthread_mutex_t *mutex,
        const struct timespec *abstime)
{
    pthread_mutex_Obj *mutexObj = (pthread_mutex_Obj *)*mutex;
    UInt32             timeout;
    long               usecs;

    if ((abstime->tv_nsec < 0) || (1000000000 < abstime->tv_nsec)) {
        return (EINVAL);
    }

    usecs = abstime->tv_sec * 1000000 + abstime->tv_nsec / 1000;
    timeout = usecs / Clock_tickPeriod;

    return (mutexAcquire(mutexObj, timeout));
}

/*
 *  ======== pthread_mutex_trylock ========
 *  Retuns:
 *    0      - Lock was successfully acquired.
 *    EINVAL - Mutex protocol is PTHREAD_PRIO_PROTECT and mutex prioceiling
 *             is lower than calling thread's priority
 *    EBUSY  - The mutex is already locked and mutex is owned by another
 *             thread or not recursive.
 *    EDEADLK - Mutex type is PTHREAD_MUTEX_ERRORCHECK and calling thread
 *              already owns the mutex.
 */
int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    pthread_mutex_Obj *mutexObj = (pthread_mutex_Obj *)*mutex;

    return (mutexAcquire(mutexObj, 0));
}

/*
 *  ======== pthread_mutex_unlock ========
 *  Retuns:
 *    0      - The mutex was successfully released.
 *    EPERM  - Calling thread does not own the mutex.
 */
int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    pthread_mutex_Obj *mutexObj = (pthread_mutex_Obj *)*mutex;
    UInt               key;

    key = Task_disable();

    if (mutexObj->owner != Task_self()) {
        Task_restore(key);
        return (EPERM);
    }

    mutexObj->lockCnt--;

    if (mutexObj->lockCnt == 0) {
        mutexObj->owner = NULL;

        Semaphore_post(Semaphore_handle(&(mutexObj->sem)));

#if defined(_POSIX_THREAD_PRIO_PROTECT)
        /* This may change the thread's priority */
        if (mutexObj->attrs.protocol == PTHREAD_PRIO_PROTECT) {
            _pthread_removeMutex(mutex);
        }
#endif
    }

    Task_restore(key);

    return (0);
}

/*
 *  ======== mutexAcquire ========
 */
static int mutexAcquire(pthread_mutex_Obj *mutex, UInt32 timeout)
{
    UInt               key;
    Task_Handle        task;

    task = Task_self();

    key = Task_disable();

    if (mutex->attrs.prioceiling < Task_getPri(task)) {
        return (EINVAL);
    }

    if (mutex->owner == NULL) {
        Assert_isTrue(mutex->lockCnt == 0, NULL);
        Assert_isTrue(Semaphore_getCount(Semaphore_handle(&(mutex->sem))) > 0,
                NULL);

        /* Take the semaphore - this should not block */
        Semaphore_pend(Semaphore_handle(&(mutex->sem)), BIOS_WAIT_FOREVER);

        mutex->owner = task;
        mutex->lockCnt++;

        Task_restore(key);
    }
    else {
        /* Return if the thread already owns the mutex */
        if (mutex->owner == task) {
            if (mutex->attrs.type == PTHREAD_MUTEX_ERRORCHECK) {
                Task_restore(key);
                return (EDEADLK);
            }
            if (mutex->attrs.type == PTHREAD_MUTEX_RECURSIVE) {
                mutex->lockCnt++;
                Task_restore(key);

                return (0);
            }
        }

        Task_restore(key);

        /*
         *  Another thread owns the mutex (or this one does and type is
         *  NORMAL).  First acquire the timeout semaphore.
         */
        if (!Semaphore_pend(Semaphore_handle(&(mutex->sem)), timeout)) {
            return ((timeout == 0) ? EBUSY : ETIMEDOUT);
        }

        Assert_isTrue(mutex->owner == NULL, NULL);
        Assert_isTrue(mutex->lockCnt == 0, NULL);

        mutex->owner = task;
        mutex->lockCnt++;
    }

#if defined(_POSIX_THREAD_PRIO_PROTECT)
    /* Set the task priority to the mutex priority ceiling */
    if (mutex->attrs.protocol == PTHREAD_PRIO_PROTECT) {
        if (mutex->lockCnt == 1) {
            _pthread_addMutex((pthread_mutex_t *)mutex);
        }
    }
#endif
    return (0);
}
