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
 *  ======== pthread_rwlock.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/gates/GateMutexPri.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include <ti/sysbios/posix/pthread.h>
#include <ti/sysbios/posix/_pthread_error.h>

#define _PTHREAD_DEBUG 1

/*
 *  ======== pthread_rwlock_Obj ========
 */
typedef struct pthread_rwlock_Obj {
    /*
     *  This semaphore must be acquired to obtain a write lock.
     *  A readlock can be obtained if there is already a read lock
     *  acquired, or by acquiring this semaphore.
     */
    Semaphore_Struct  sem;

    /*
     *  This semaphore is used to block readers when sem is in use
     *  by a write lock.
     */
    Semaphore_Struct  readSem;

    int       activeReaderCnt;   /* Number of read locks acquired */
    int       blockedReaderCnt;  /* Number of readers blocked on readSem */

#ifdef _PTHREAD_DEBUG
    /*
     *  For debugging and testing.  The 'owner' is the writer holding
     *  to lock, or the first reader that acquired the lock.
     */
    pthread_t owner;
#endif
} pthread_rwlock_Obj;

static int rdlockAcquire(pthread_rwlock_Obj *lock, UInt timeout);

/*
 *************************************************************************
 *                      pthread_rwlockattr
 *************************************************************************
 */
/*
 *  ======== pthread_rwlockattr_destroy ========
 */
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr)
{
    return (0);
}

/*
 *  ======== pthread_rwlockattr_getpshared ========
 */
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *attr,
        int *pshared)
{
    *pshared = attr->pshared;
    return (0);
}

/*
 *  ======== pthread_rwlockattr_init ========
 */
int pthread_rwlockattr_init(pthread_rwlockattr_t * attr)
{
    attr->pshared = PTHREAD_PROCESS_PRIVATE;
    return (0);
}

/*
 *  ======== pthread_rwlockattr_setpshared ========
 */
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared)
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
 *                      pthread_rwlock
 *************************************************************************
 */

/*
 *  ======== pthread_rwlock_destroy ========
 */
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_Obj *lockObj = (pthread_rwlock_Obj *)*rwlock;

    Semaphore_destruct(&(lockObj->sem));
    Semaphore_destruct(&(lockObj->readSem));

    Memory_free(Task_Object_heap(), lockObj, sizeof(pthread_rwlock_Obj));

    *rwlock = NULL;
    return (0);
}

/*
 *  ======== pthread_rwlock_init ========
 */
int pthread_rwlock_init(pthread_rwlock_t *rwlock,
        const pthread_rwlockattr_t *attr)
{
    pthread_rwlock_Obj *lockObj;
    Semaphore_Params    semParams;
    Error_Block         eb;

    Error_init(&eb);

    // TODO: Is Task_Object_heap() ok to use?
    lockObj = (pthread_rwlock_Obj *)Memory_alloc(Task_Object_heap(),
            sizeof(pthread_rwlock_Obj), 0, &eb);

    if (lockObj == NULL) {
        return (ENOMEM);
    }

    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_COUNTING;

    Semaphore_construct(&(lockObj->sem), 1, &semParams);
    Semaphore_construct(&(lockObj->readSem), 0, &semParams);

    lockObj->activeReaderCnt = 0;
    lockObj->blockedReaderCnt = 0;

    *rwlock = (void *)lockObj;

#ifdef _PTHREAD_DEBUG
    lockObj->owner = (pthread_t)NULL;
#endif

    return (0);
}

/*
 *  ======== pthread_rwlock_rdlock ========
 */
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_Obj *lockObj = (pthread_rwlock_Obj *)*rwlock;

    return (rdlockAcquire(lockObj, BIOS_WAIT_FOREVER));
}

/*
 *  ======== pthread_rwlock_timedrdlock ========
 */
int pthread_rwlock_timedrdlock(pthread_rwlock_t *rwlock,
        const struct timespec *abstime)
{
    pthread_rwlock_Obj *lockObj = (pthread_rwlock_Obj *)*rwlock;
    UInt32              timeout;
    long                usecs;

    usecs = abstime->tv_sec * 1000000 + abstime->tv_nsec / 1000;
    timeout = usecs / Clock_tickPeriod;

    return (rdlockAcquire(lockObj, timeout));

}

/*
 *  ======== pthread_rwlock_timedwrlock ========
 */
int pthread_rwlock_timedwrlock(pthread_rwlock_t *rwlock,
        const struct timespec *abstime)
{
    pthread_rwlock_Obj *lockObj = (pthread_rwlock_Obj *)*rwlock;
    UInt32              timeout;
    long                usecs;

    usecs = abstime->tv_sec * 1000000 + abstime->tv_nsec / 1000;
    timeout = usecs / Clock_tickPeriod;

    if (Semaphore_pend(Semaphore_handle(&(lockObj->sem)), timeout)) {
#ifdef _PTHREAD_DEBUG
        Assert_isTrue(lockObj->owner == (pthread_t)NULL, NULL);
        lockObj->owner = pthread_self();
#endif
        Assert_isTrue(lockObj->activeReaderCnt == 0, NULL);

        return (0);
    }

    return (EBUSY);
}

/*
 *  ======== pthread_rwlock_tryrdlock ========
 */
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_Obj *lockObj = (pthread_rwlock_Obj *)*rwlock;

    return (rdlockAcquire(lockObj, 0));
}

/*
 *  ======== pthread_rwlock_trywrlock ========
 */
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_Obj *lockObj = (pthread_rwlock_Obj *)*rwlock;

    if (Semaphore_pend(Semaphore_handle(&(lockObj->sem)), 0)) {
#ifdef _PTHREAD_DEBUG
        Assert_isTrue(lockObj->owner == (pthread_t)NULL, NULL);
        lockObj->owner = pthread_self();
#endif
        Assert_isTrue(lockObj->activeReaderCnt == 0, NULL);

        return (0);
    }

    return (EBUSY);
}

/*
 *  ======== pthread_rwlock_unlock ========
 */
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_Obj *lockObj = (pthread_rwlock_Obj *)*rwlock;
    UInt                key;

    key = Task_disable();

    if (lockObj->activeReaderCnt) {
        Assert_isTrue(lockObj->blockedReaderCnt == 0, NULL);
        /*
         *  Lock is held by a reader.  The last active reader
         *  releases the semaphore.
         */
        if (--lockObj->activeReaderCnt == 0) {
            Semaphore_post(Semaphore_handle(&(lockObj->sem)));
        }

        Task_restore(key);
    }
    else {
        /*
         *  Lock is held by a writer.  Release the semaphore and
         *  if there are any blocked readers, unblock one of them.
         */
        Semaphore_post(Semaphore_handle(&(lockObj->sem)));
        if (lockObj->blockedReaderCnt > 0) {
            Semaphore_post(Semaphore_handle(&(lockObj->readSem)));
        }

#ifdef _PTHREAD_DEBUG
        Assert_isTrue(lockObj->owner == pthread_self(), NULL);
        lockObj->owner = (pthread_t)NULL;
#endif
        Assert_isTrue(lockObj->activeReaderCnt == 0, NULL);

        Task_restore(key);
    }

    return (0);
}

/*
 *  ======== pthread_rwlock_wrlock ========
 */
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_Obj *lockObj = (pthread_rwlock_Obj *)*rwlock;

    Semaphore_pend(Semaphore_handle(&(lockObj->sem)), BIOS_WAIT_FOREVER);

#ifdef _PTHREAD_DEBUG
    Assert_isTrue(lockObj->owner == (pthread_t)NULL, NULL);
    lockObj->owner = pthread_self();
#endif
    Assert_isTrue(lockObj->activeReaderCnt == 0, NULL);

    return (0);
}


/*
 *************************************************************************
 *                      internal functions
 *************************************************************************
 */

/*
 *  ======== rdlockAcquire ========
 */
static int rdlockAcquire(pthread_rwlock_Obj *lockObj, UInt timeout)
{
    UInt32              curTicks;
    UInt32              prevTicks;
    UInt32              deltaTicks;
    UInt                key;
    int                 i;

    key = Task_disable();

    curTicks = Clock_getTicks();

    if (lockObj->activeReaderCnt > 0) {
        /* The semaphore is owned by a reader, no need to pend. */
        lockObj->activeReaderCnt++;
        Task_restore(key);

        return (0);
    }

    if (Semaphore_pend(Semaphore_handle(&(lockObj->sem)), 0)) {
        /* Got the semaphore */
        lockObj->activeReaderCnt++;
        Assert_isTrue(lockObj->activeReaderCnt == 1, NULL);

#ifdef _PTHREAD_DEBUG
        Assert_isTrue(lockObj->owner == (pthread_t)NULL, NULL);
        lockObj->owner = pthread_self();
#endif
        /* Unblock all other readers */
        for (i = 0; i < lockObj->blockedReaderCnt; i++) {
            Semaphore_post(Semaphore_handle(&(lockObj->readSem)));
        }
        Semaphore_reset(Semaphore_handle(&(lockObj->readSem)), 0);

        Task_restore(key);

        return (0);
    }

    if (timeout == 0) {
        Task_restore(key);
        return (EBUSY);
    }

    lockObj->blockedReaderCnt++;
    Task_restore(key);

    for (;;) {
        if (!Semaphore_pend(Semaphore_handle(&(lockObj->readSem)), timeout)) {
            key = Task_disable();
            lockObj->blockedReaderCnt--;
            Task_restore(key);

            return (EBUSY);
        }

        key = Task_disable();

        /*
         *  If another reader is active, the lockObj->sem is owned
         *  by a reader, so just increment the active reader count.
         */
        if (lockObj->activeReaderCnt > 0) {
            lockObj->blockedReaderCnt--;
            lockObj->activeReaderCnt++;
            Task_restore(key);

            return (0);
        }

        /*
         *  We have been unblocked by a writer.  Try again to take the
         *  lockObj->sem, since another writer or reader may have taken
         *  it in the meantime.
         */
        if (Semaphore_pend(Semaphore_handle(&(lockObj->sem)), 0)) {
            /* Got it */
            lockObj->blockedReaderCnt--;
            lockObj->activeReaderCnt++;

            /* Unblock all other readers */
            for (i = 0; i < lockObj->blockedReaderCnt; i++) {
                Semaphore_post(Semaphore_handle(&(lockObj->readSem)));
            }
            Semaphore_reset(Semaphore_handle(&(lockObj->readSem)), 0);

            Task_restore(key);
            return (0);
        }

        if (timeout != BIOS_WAIT_FOREVER) {
            prevTicks = curTicks;
            curTicks = Clock_getTicks();
            deltaTicks = curTicks - prevTicks;

            if (deltaTicks >= timeout) {
                /* Timed out without acquiring the lock */
                Task_restore(key);
                break;
            }
            timeout -= deltaTicks;
        }

        Task_restore(key);
    }

    return (EBUSY);
}

/*
 *************************************************************************
 *              internal functions for testing
 *************************************************************************
 */

#ifdef _PTHREAD_DEBUG

pthread_t pthread_rwlock_getowner(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_Obj *lockObj = (pthread_rwlock_Obj *)*rwlock;

    return (lockObj->owner);
}

int pthread_rwlock_getActiveReaderCnt(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_Obj *lockObj = (pthread_rwlock_Obj *)*rwlock;

    return (lockObj->activeReaderCnt);
}

int pthread_rwlock_getBlockedReaderCnt(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_Obj *lockObj = (pthread_rwlock_Obj *)*rwlock;

    return (lockObj->blockedReaderCnt);
}

#endif
