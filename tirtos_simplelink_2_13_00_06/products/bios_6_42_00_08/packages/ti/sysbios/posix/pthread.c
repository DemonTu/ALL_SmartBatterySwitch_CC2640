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
 *  ======== pthread.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/sysbios/posix/pthread.h>
#include <ti/sysbios/posix/_pthread.h>
#include <ti/sysbios/posix/_pthread_error.h>

typedef void *(*pthread_RunFxn)(void *);

/*
 *  ======== pthread_Obj ========
 */
typedef struct pthread_Obj {
    Task_Handle       task;
    Semaphore_Struct  joinSem;
    Semaphore_Struct  doneSem;
#if defined(_POSIX_THREAD_PRIO_PROTECT)
    /*
     *  When a thread acquires a PTHREAD_PRIO_PROTECT mutex, the thread's
     *  priority will be bumped to the priority ceiling of the mutex, if
     *  that is higher than the thread's priority.  When the thread
     *  releases the mutex, its priority will be set to the max of the
     *  priority ceilings of the mutexes it still owns, and its original
     *  priority.  So we need to keep track of its acquired mutexes and
     *  original priority before acquiring any mutexes.
     */
    Queue_Struct      mutexList;
    int               priority;
#endif

    pthread_t         joinThread;

    int               detached;
    pthread_RunFxn    fxn;
    void             *arg;
    int               detachstate;
    int               cancelState;
    int               cancelType;
    int               cancelPending;

    /* Thread function return value */
    void             *ret;
} pthread_Obj;

static int _pthread_getMaxPrioCeiling(pthread_Obj *thread);
static void _pthread_runStub(UArg arg0, UArg arg1);

/*
 *************************************************************************
 *                      pthread_attr
 *************************************************************************
 */
/*
 *  ======== pthread_attr_destroy ========
 */
int pthread_attr_destroy(pthread_attr_t *attr)
{
    return (0);
}

/*
 *  ======== pthread_attr_getdetachstate ========
 */
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate)
{
    *detachstate = attr->detachstate;
    return (0);
}

/*
 *  ======== pthread_attr_getguardsize ========
 */
int pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize)
{
    *guardsize = attr->guardsize;
    return (0);
}

/*
 *  ======== pthread_attr_getschedparam ========
 */
int pthread_attr_getschedparam(const pthread_attr_t *attr,
        struct sched_param *schedparam)
{
    schedparam->sched_priority = attr->priority;
    return (0);
}

/*
 *  ======== pthread_attr_getstack ========
 */
int pthread_attr_getstack (const pthread_attr_t *attr, void **stackaddr,
        size_t *stacksize)
{
    *stackaddr = attr->stack;
    *stacksize = attr->stacksize;
    return (0);
}

/*
 *  ======== pthread_attr_getstackaddr ========
 */
int pthread_attr_getstackaddr(const pthread_attr_t *attr, void ** stackaddr)
{
    *stackaddr = attr->stack;
    return (0);
}

/*
 *  ======== pthread_attr_getstacksize ========
 */
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize)
{
    *stacksize = attr->stacksize;
    return (0);
}

/*
 *  ======== pthread_attr_init ========
 */
int pthread_attr_init(pthread_attr_t *attr)
{
    attr->priority = 1;
    attr->stack = NULL;
    attr->stacksize = Task_defaultStackSize;
    attr->guardsize = 0;
    attr->detachstate = PTHREAD_CREATE_JOINABLE;

    return (0);
}

/*
 *  ======== pthread_attr_setdetachstate ========
 */
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
    attr->detachstate = detachstate;
    return (0);
}

/*
 *  ======== pthread_attr_setguardsize ========
 */
int   pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize)
{
    attr->guardsize = guardsize;
    return (0);
}

/*
 *  ======== pthread_attr_setguardsize ========
 */
int pthread_attr_setschedparam(pthread_attr_t *attr,
        const struct sched_param *schedparam)
{
    attr->priority = schedparam->sched_priority;
    return (0);
}

/*
 *  ======== pthread_attr_setstack ========
 */
int pthread_attr_setstack (pthread_attr_t *attr, void *stackaddr,
        size_t stacksize)
{
    attr->stack = stackaddr;
    attr->stacksize = stacksize;
    return (0);
}

/*
 *  ======== pthread_attr_setstackaddr ========
 */
int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr)
{
    attr->stack = stackaddr;
    return (0);
}

/*
 *  ======== pthread_attr_setstacksize ========
 */
int   pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
    attr->stacksize = stacksize;
    return (0);
}

/*
 *************************************************************************
 *                      pthread
 *************************************************************************
 */

/*
 *  ======== pthread_cancel ========
 */
int pthread_cancel(pthread_t pthread)
{
    pthread_Obj *thread = (pthread_Obj *)pthread;
    UInt key;

    /*
     *  Send cancellation request to the thread.
     *  If the thread's cancelability is enabled, then if the cancelability
     *  type is asynchronous, the system can cancel the thread at any
     *  time (usually immediately, but system doesn't have to guarantee this).
     *
     *  If the cancelability type is deferred, the thread cannot be
     *  cancelled until it calls a cancellation point (e.g.,
     *  pthread_testcancel()).
     */
    key = Hwi_disable();

    /* Indicate that cancellation is requested. */
    thread->cancelPending = 1;

    if (thread->cancelState == PTHREAD_CANCEL_ENABLE) {
        if (thread->cancelType == PTHREAD_CANCEL_ASYNCHRONOUS) {
            /*
             *  Asynchronous cancellation type.
             *
             *  TODO: Set the thread->ret to PTHREAD_CANCELED
             *  Pop the cleanup handlers.
             *  Terminate the thread.
             */
        }
    }

    Hwi_restore(key);

    return (0);
}

/*
 *  ======== pthread_create ========
 */
int pthread_create(pthread_t *newthread, const pthread_attr_t *attr,
        void *(*startroutine)(void *), void *arg)
{
    Semaphore_Params  semParams;
    Task_Params       taskParams;
    pthread_Obj      *thread = NULL;
    int               detachstate = PTHREAD_CREATE_JOINABLE;
    Error_Block       eb;
    int               failedStatus = 0;

    Error_init(&eb);
    Task_Params_init(&taskParams);

    if (attr) {
        taskParams.priority = attr->priority;
        taskParams.stack = attr->stack;
        taskParams.stackSize = attr->stacksize;
        detachstate = attr->detachstate;
    }

    thread = (pthread_Obj *)Memory_alloc(Task_Object_heap(),
            sizeof(pthread_Obj), 0, &eb);

    if (thread != NULL) {
        thread->detached = (detachstate == PTHREAD_CREATE_JOINABLE) ? 0 : 1;
        thread->fxn = startroutine;
        thread->joinThread = (pthread_t)NULL;
        thread->arg = arg;
        thread->cancelState = PTHREAD_CANCEL_ENABLE;
        thread->cancelType = PTHREAD_CANCEL_DEFERRED;
        thread->cancelPending = 0;
        thread->priority = taskParams.priority;

#if defined(_POSIX_THREAD_PRIO_PROTECT)
        Queue_construct(&(thread->mutexList), NULL);
#endif
        Semaphore_Params_init(&semParams);
        semParams.mode = Semaphore_Mode_BINARY;

        Semaphore_construct(&(thread->joinSem), 0, &semParams);
        Semaphore_construct(&(thread->doneSem), 0, &semParams);

        taskParams.arg0 = (UArg)thread;
        thread->task = Task_create((Task_FuncPtr)_pthread_runStub,
                &taskParams, &eb);

        if (thread->task == NULL) {
            failedStatus = 1;
        }
    }
    else {
        failedStatus = ENOMEM;
    }

    /* Failure */
    if (failedStatus) {
        if (thread) {
            Semaphore_destruct(&(thread->joinSem));
            Semaphore_destruct(&(thread->doneSem));

            if (thread->task) {
                Task_delete(&thread->task);
            }

            Memory_free(Task_Object_heap(), thread, sizeof(pthread_Obj));
            thread = NULL;
        }
    }

    *newthread = (pthread_t)thread;

    return (failedStatus);
}

/*
 *  ======== pthread_detach ========
 */
int pthread_detach(pthread_t pthread)
{
    pthread_Obj *thread = (pthread_Obj *)pthread;
    UInt         key;

    key = Hwi_disable();

    if (thread->detachstate != PTHREAD_CREATE_JOINABLE) {
        Hwi_restore(key);
        /* Only threads created in the joinable state can be detached. */
        return (EINVAL);
    }

    if ((thread->joinThread != (pthread_t)NULL) || (thread->detached)) {
        Hwi_restore(key);

        /*
         *  A thread has already called pthread_join() or
         *  pthread_detach() for this thread.
         */
        return (2);  // TODO: Find error code.
    }

    /*
     *  pthread_detach() marks the thread as detached.  When
     *  the thread terminates, its resources will automatically
     *  be freed without the need for another thread to call
     *  pthread_join().
     */
    thread->detached = 1;

    Hwi_restore(key);

    return (0);
}

/*
 *  ======== pthread_exit ========
 */
void pthread_exit(void *retval)
{
    pthread_Obj      *thread = (pthread_Obj *)pthread_self();

    /*
     *  This function terminates the calling thread and returns
     *  a value via retval that (if the thread is joinable) is available to
     *  another thread that calls pthread_join().
     *
     *  Any clean-up handlers established by pthread_cleanup_push() that
     *  have not yet been popped, are popped (in the reverse of the order in
     *  which they were pushed) and executed.
     */
    thread->ret = retval;

    if (!thread->detached) {
        Semaphore_post(Semaphore_handle(&(thread->joinSem)));
        Semaphore_pend(Semaphore_handle(&(thread->doneSem)), BIOS_WAIT_FOREVER);
    }

    /* TODO: Pop cleanup handlers */

    /* The Task_Object will be freed in the Idle loop. */
    Memory_free(Task_Object_heap(), thread, sizeof(pthread_Obj));
    Task_exit();
}

/*
 *  ======== pthread_getcpuclockid ========
 */
int pthread_getcpuclockid (pthread_t __pthread_id,
        clockid_t *__clock_id)
{
    /* TODO */
    return (1);
}

/*
 *  ======== pthread_getschedparam ========
 */
int pthread_getschedparam(pthread_t pthread, int *policy,
        struct sched_param *param)
{
    pthread_Obj      *thread = (pthread_Obj *)pthread;

    *policy = SCHED_OTHER;
    param->sched_priority = Task_getPri(thread->task);

    //  TODO: If the task priority is higher because of holding
    //  a PTHREAD_PRIO_PROTECT mutex, which priority do we return?

    return (0);
}

/*
 *  ======== pthread_getspecific ========
 */
void *pthread_getspecific(pthread_key_t key)
{
    /* TODO */
    return (NULL);
}

/*
 *  ======== pthread_join ========
 *  Wait for thread to terminate.
 *
 *  If multiple threads simultaneously try to join with the same
 *  thread, the results are undefined.  We will return an error.
 *
 *  If the thread calling pthread_join() is canceled, then the target
 *  thread will remain joinable (i.e., it will not be detached).
 */
int pthread_join(pthread_t pthread, void **thread_return)
{
    pthread_Obj      *thread = (pthread_Obj *)pthread;
    UInt              key;

    key = Hwi_disable();

    if ((thread->joinThread != (pthread_t)NULL) || (thread->detached != 0)) {
        /*
         *  Error - Another thread has already called pthread_join()
         *  for this thread, or the thread is in the detached state.
         */
        Hwi_restore(key);
        return (1);       // TODO: Find error code.
    }

    thread->joinThread = pthread_self();

    Hwi_restore(key);

    Semaphore_pend(Semaphore_handle(&(thread->joinSem)), BIOS_WAIT_FOREVER);

    if (thread_return) {
        /*
         *  Must be set to PTHREAD_CANCELED if the thread was
         *  canceled.
         */
        *thread_return = thread->ret;
    }

    /* Post the thread's doneSem to allow it the be freed. */
    Semaphore_post(Semaphore_handle(&(thread->doneSem)));

    return (0);
}

/*
 *  ======== pthread_key_create ========
 */
int pthread_key_create(pthread_key_t *key,
        void (*__destructor)(void *))
{
    /*
     *  TODO:  Do we want to implement this for BIOS?  Would
     *  need to allocate a table for mapping of thread Id's to
     *  key values.
     */
    return (1);
}

/*
 *  ======== pthread_key_delete ========
 */
int pthread_key_delete(pthread_key_t key)
{
   /* TODO - May not want to implement this for BIOS. */
    return (1);
}

/*
 *  ======== pthread_self ========
 */
pthread_t pthread_self(void)
{
    pthread_Obj *thread;
    Task_Handle  task = Task_self();

    thread = (pthread_Obj *)Task_getArg0(task);
    return ((pthread_t)thread);
}

/*
 *  ======== pthread_setcancelstate ========
 */
int pthread_setcancelstate(int state, int *oldstate)
{
    pthread_Obj *thread = (pthread_Obj *)pthread_self();
    UInt         key;

    key = Hwi_disable();

    *oldstate = thread->cancelState;
    thread->cancelState = state;

    Hwi_restore(key);

    return (0);
}

/*
 *  ======== pthread_setcanceltype ========
 */
int pthread_setcanceltype(int canceltype, int *oldtype)
{
    pthread_Obj *thread = (pthread_Obj *)pthread_self();
    UInt         key;

    key = Hwi_disable();

    *oldtype = thread->cancelType;
    thread->cancelType = canceltype;

    Hwi_restore(key);

    return (0);
}

/*
 *  ======== pthread_setconcurrency ========
 */
int pthread_setconcurrency(int concurrency)
{
    /* Not applicable to BIOS */
    return (1);
}

/*
 *  ======== pthread_setschedparam ========
 */
int pthread_setschedparam(pthread_t pthread, int policy,
        const struct sched_param *param)
{
    pthread_Obj        *thread = (pthread_Obj *)pthread;
    Task_Handle         task = thread->task;
    UInt                oldPri;
    int                 priority = param->sched_priority;
#if defined(_POSIX_THREAD_PRIO_PROTECT)
    int                 maxPri;
    UInt                key;
#endif

    if ((priority >= Task_numPriorities) ||
            ((priority == 0) && (Task_getIdleTask() != NULL)) ||
            (priority < -1)) {
        /* Bad priority value */
        return (EINVAL);
    }

#if defined(_POSIX_THREAD_PRIO_PROTECT)

    /*
     *  If the thread is holding a PTHREAD_PRIO_PROTECT mutex and
     *  running at its ceiling, we don't want to set its priority
     *  to a lower value.  Instead, we save the new priority to set
     *  it to, once the mutexes of higher priority ceilings are
     *  released.
     */
    key = Task_disable();

    oldPri = Task_getPri(task);
    thread->priority = priority;

    if (!Queue_empty(Queue_handle(&(thread->mutexList)))) {
        maxPri = _pthread_getMaxPrioCeiling(thread);

        if (priority > maxPri) {
            Task_setPri(task, priority);
        }
    }
    else {
        Task_setPri(task, priority);
    }

    Task_restore(key);

#else
    oldPri = Task_setPri(task, priority);
#endif

    /* Suppress warning about oldPri not being used. */
    (void)oldPri;

    return (0);
}

/*
 *  ======== pthread_setspecific ========
 */
int pthread_setspecific(pthread_key_t key, const void *value)
{
    /* TODO - May not want to implement this for BIOS. */
    return (1);
}

/*
 *  ======== pthread_testcancel ========
 */
void pthread_testcancel(void)
{
    pthread_Obj *thread = (pthread_Obj *)pthread_self();

    /* Act on a cancellation request. */

    if (thread->cancelPending) {
        thread->ret = PTHREAD_CANCELED;

        if (thread->detachstate == PTHREAD_CREATE_JOINABLE) {
            Semaphore_post(Semaphore_handle(&(thread->joinSem)));
            Semaphore_pend(Semaphore_handle(&(thread->doneSem)),
                    BIOS_WAIT_FOREVER);
        }

        /* TODO: pop and run the cleanup handlers */

        Memory_free(Task_Object_heap(), thread, sizeof(pthread_Obj));

        Task_exit();
    }
}

#if defined(_POSIX_THREAD_PRIO_PROTECT)

/*
 *  ======== _pthread_addMutex ========
 *  Add a PTHREAD_PRIO_PROTECT mutex to the queue and bump the thread's
 *  priority, if necessary.
 */
void _pthread_addMutex(pthread_mutex_t *mutex)
{
    pthread_Obj        *thread = (pthread_Obj *)pthread_self();
    int                 priority;
    int                 prioceiling;
    int                 key;

    key = Task_disable();

    Queue_enqueue(Queue_handle(&(thread->mutexList)), (Queue_Elem *)mutex);

    pthread_mutex_getprioceiling(mutex, &prioceiling);

    priority = Task_getPri(thread->task);
    if (priority < prioceiling) {
        Task_setPri(thread->task, prioceiling);
    }

    Task_restore(key);
}

/*
 *  ======== _pthread_getMaxPrioCeiling ========
 *  Return the maximum of the priority ceilings of the PTHREAD_PRIO_PROTECT
 *  mutexes owned by the thread.
 */
static int _pthread_getMaxPrioCeiling(pthread_Obj *thread)
{
    pthread_mutex_t    *mutex;
    int                 maxPri = 0;
    int                 pri;
    UInt                key;

    /*
     *  If the thread is holding a PTHREAD_PRIO_PROTECT mutex and
     *  running at its ceiling, we don't want to set its priority
     *  to a lower value.  Instead, we save the new priority to set
     *  it to, once the mutexes of higher priority ceilings are
     *  released.
     */
    key = Task_disable();

    mutex = Queue_head(Queue_handle(&(thread->mutexList)));

    while (mutex != (pthread_mutex_t *)Queue_handle(&thread->mutexList)) {
        pthread_mutex_getprioceiling(mutex, &pri);
        maxPri = (pri > maxPri) ? pri : maxPri;
    }

    Task_restore(key);

    return (maxPri);
}

/*
 *  ======== _pthread_removeMutex ========
 *  Remove a PTHREAD_PRIO_PROTECT mutex from the queue and restore
 *  priority, if necessary.
 */
void _pthread_removeMutex(pthread_mutex_t *mutex)
{
    pthread_Obj        *thread = (pthread_Obj *)pthread_self();
    int                 maxPri;
    int                 priority;
    int                 key;

    key = Task_disable();

    Queue_remove((Queue_Elem *)mutex);
    maxPri = _pthread_getMaxPrioCeiling(thread);
    priority = thread->priority;

    /*
     *  Take the larger of the thread's priority and the ceilings of
     *  the remaining mutexes it owns.
     */
    priority = (priority > maxPri) ? priority : maxPri;

    Task_setPri(thread->task, priority);

    Task_restore(key);
}

#endif

/*
 *  ======== _pthread_runStub ========
 */
static void _pthread_runStub(UArg arg0, UArg arg1)
{
    pthread_Obj *thread = (pthread_Obj *)arg0;

    thread->ret = thread->fxn((void *)thread->arg);

    if (!thread->detached) {
        Semaphore_post(Semaphore_handle(&(thread->joinSem)));
        Semaphore_pend(Semaphore_handle(&(thread->doneSem)), BIOS_WAIT_FOREVER);
    }

#if defined(_POSIX_THREAD_PRIO_PROTECT)
        Queue_destruct(&(thread->mutexList));
#endif
    /*
     *  The Task_Object will be freed in the Idle loop.
     */
    Memory_free(Task_Object_heap(), thread, sizeof(pthread_Obj));
}
