/*
 * Copyright (c) 2012-2015, Texas Instruments Incorporated
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
 *  ======== Power.xdc ========
 *
 *
 */

package ti.sysbios.family.arm.cc26xx;

import xdc.rov.ViewInfo;

import xdc.runtime.Assert;
import ti.sysbios.knl.Queue;
import ti.sysbios.knl.Clock;

/*!
 *  ======== Power ========
 *  CC26xx Power Manager
 *
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center"></colgroup>
 *
 *    <tr><th> Function                 </th><th>  Hwi   </th><th>  Swi   </th><th>  Task  </th><th>  Main  </th><th>  Startup  </th></tr>
 *    <!-- -->
 *    <tr><td> {@link #getConstraintInfo}  </td><td>   Y    </td><td>   Y    </td><td>   Y   </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #getDependencyCount}  </td><td>   Y    </td><td>   Y    </td><td>   Y   </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #getTicksUntilWakeup}  </td><td>   N    </td><td>   N    </td><td>   Y   </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getTransitionLatency}  </td><td>   Y    </td><td>   Y    </td><td>   Y   </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #getTransitionState}  </td><td>   Y    </td><td>   Y    </td><td>   Y   </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #getXoscStartupTime}  </td><td>   Y    </td><td>   Y    </td><td>   Y   </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #registerNotify}  </td><td>   N    </td><td>   N    </td><td>   Y   </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #releaseConstraint}  </td><td>   Y    </td><td>   Y    </td><td>   Y   </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #releaseDependency}  </td><td>   Y    </td><td>   Y    </td><td>   Y   </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #setConstraint}  </td><td>   Y    </td><td>   Y    </td><td>   Y   </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #setDependency}  </td><td>   Y    </td><td>   Y    </td><td>   Y   </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #shutdown}  </td><td>   N    </td><td>   N    </td><td>   Y   </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #sleep}  </td><td>   N    </td><td>   N    </td><td>   Y   </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #unregisterNotify}  </td><td>   N    </td><td>   N    </td><td>   Y   </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td colspan="6"> Definitions: <br />
 *       <ul>
 *         <li> <b>Hwi</b>: API is callable from a Hwi thread </li>
 *         <li> <b>Swi</b>: API is callable from a Swi thread </li>
 *         <li> <b>Task</b>: API is callable from a Task thread </li>
 *         <li> <b>Main</b>: API is callable from main() </li>
 *         <li> <b>Startup</b>: API is callable during any module startup </li>
 *         <li> <b>Y</b>: Yes </li>
 *         <li> <b>Y*</b>: Yes, but see API description for restrictions </li>
 *         <li> <b>N</b>: No </li>
 *       </ul>
 *    </td></tr>
 *
 *  </table>
 *  @p
 */

@ModuleStartup            /* Initialize Power */
@Template("./Power.xdt")  /* Template for contitional initiatialization calls */

module Power inherits ti.sysbios.interfaces.IPower
{
    /*! @_nodoc Idle function prototype */
    typedef Void (*FuncPtr)();

    /*!
     *  ======== Constraint ========
     *  Constraints that can be registered with Power
     */
    enum Constraint {
        SB_VIMS_CACHE_RETAIN      = 0x1, /*! disallow loss of cache context during Power_STANDBY (i.e., retain the cache) */
        SD_DISALLOW               = 0x2, /*! disallow transition to SHUTDOWN */
        SB_DISALLOW               = 0x4,/*! disallow transition to Power_STANDBY */
        IDLE_PD_DISALLOW          = 0x8,/*! disallow power down of the CPU domain during idle time (IDLE_PD) */
        NEED_FLASH_IN_IDLE        = 0x10,/*! need to keep Flash powered in IDLE_PD */
        NUMCONSTRAINTS = 5
    };

    /*!
     *  ======== LatencyType ========
     *  Type of transition latency to query
     */
    enum LatencyType {
        TOTAL,   /*! the total latency, to enter the sleep state, and to wake and resume */
        RESUME   /*! the latency to wake the device and resume execution */
    };

    /*!
     *  ======== SleepState ========
     *  Sleep states
     */
    enum SleepState {
        STANDBY = 0x1   /*! STANDBY state */
    };

    /*!
     *  ======== Event ========
     *  Power Event enumerations
     */
    enum Event {
        ENTERING_STANDBY          = 0x1, /*! device is transitioning into Power_STANDBY */
        ENTERING_SHUTDOWN         = 0x2, /*! device is transitioning into SHUTDOWN */
        AWAKE_STANDBY             = 0x4, /*! device has awoken from Power_STANDBY, I/O latches are still frozen */
        AWAKE_STANDBY_LATE        = 0x8, /*! device has awoken from Power_STANDBY, I/O latches are open again */
        XOSC_HF_SWITCHED          = 0x10, /*! the high frequency (HF) crystal oscillator has stabilized, and is now the HF clock source */
        NUMEVENTS = 5
    };

    /*!
     *  ======== NotifyResponse ========
     *  Return values from a notify function.
     */
    enum NotifyResponse {
        NOTIFYDONE = 0,/*! success, done processing the notification */
        NOTIFYERROR   /*! an error occurred while processing the notification */
    };

    /*!
     *  ======== Status ========
     *  Function return codes
     */
    enum Status {
        SOK = 0,                /*! success */
        SOK_STATE_RESTORED = 1, /*! success, context has been restored */
        EFAIL = 2,              /*! error, the operation failed */
        EINVALIDPOINTER = 3,    /*! error, an invalid pointer was specified */
        ECHANGE_NOT_ALLOWED = 4,/*! error, the requested change is not allowed */
        ETIMEOUT = 5,           /*! error, a timeout occurred */
        EBUSY = 6               /*! error, the operation is not allowed; another transition is not completed yet */
    };

    /*!
     *  ======== TransitionState ========
     *  Sleep transition states
     */
    enum TransitionState {
        ACTIVE = 0x1,        /*! device is active; no transition in progress */
        ENTERING_SLEEP = 0x2,/*! device is entering into a sleep state */
        EXITING_SLEEP = 0x3, /*! device is exiting a sleep state */
        SHUTDOWN = 0x4 /*! device is transitioning to the SHUTDOWN state */
    };

    /*! @_nodoc Notification object structure. */
    struct NotifyObj {
        Queue.Elem link;
        UInt32 eventTypes;
        Fxn notifyFxn;
        UArg clientArg;
    };

    // Asserts

    /*! Assert when Power_releaseConstraint is called too many times */
    config Assert.Id A_tooManyCallsReleaseConstraint = {
        msg: "A_tooManyCallsReleaseConstraint: Power_releaseConstraint() called too many times for this constraint."
    };

    /*! Assert when Power_releaseDependency is called too many times */
    config Assert.Id A_tooManyCallsReleaseDependency = {
        msg: "A_tooManyCallsReleaseDependency: Power_releaseDependency() called too many times for this resource."
    };

    // configs

    /*!
     *  ======== idle ========
     *  Idle the CPU during idle time?
     *
     *  When this configuration parameter is set to true, the Power module will
     *  insert a power policy function into the list of {@link
     *  ti.sysbios.knl.Idle Idle} loop functions.
     *  When the policy function executes, it will take action to save power
     *  until the next CPU interrupt occurs.  The default power policy will
     *  simply invoke a wait for interrupt (WFI) instruction.
     *
     *  @see #policyFunc
     *  @see #doWFI
     *  @see #standbyPolicy
     */
    override config Bool idle = false;

    /*!
     *  @_nodoc
     *  ======== resumeSTANDBY ========
     *  Latency to resume from STANDBY (in microseconds).
     *  NB: this is a temporary value pending further characterization.
     */
    metaonly config UInt resumeSTANDBY = 750;

    /*!
     *  @_nodoc
     *  ======== resumeTicksSTANDBY ========
     *  Latency to resume from STANDBY (in units of Clock ticks).
     */
    config UInt resumeTicksSTANDBY;

    /*!
     *  @_nodoc
     *  ======== totalSTANDBY ========
     *  Total latency to enter and resume from STANDBY (in microseconds).
     *  NB: this is a temporary value pending further characterization.
     */
    metaonly config UInt totalSTANDBY = 1000;

    /*!
     *  @_nodoc
     *  ======== totalTicksSTANDBY ========
     *  Total latency to enter and resume from STANDBY (in units of Clock
     *  ticks).
     */
    config UInt totalTicksSTANDBY;

    /*!
     *  @_nodoc
     *  ======== wakeDelaySTANDBY ========
     *  Device initial wakeup latency from STANDBY (in units of microseconds).
     */
    config UInt wakeDelaySTANDBY = 130;

    /*!
     *  @_nodoc
     *  ======== initialWaitXOSC_HF ========
     *  Time (in units of usec) to wait to see if XOSC_HF is stable.
     */
    config UInt initialWaitXOSC_HF = 50;

    /*!
     *  @_nodoc
     *  ======== retryWaitXOSC_HF ========
     *  Time (in units of usec) to wait when retrying to see if XOSC_HF is
     *   stable.
     */
    config UInt retryWaitXOSC_HF = 50;

    /*!
     *  @_nodoc
     *  ======== calibrateRCOSC ========
     *  Enable calibration of RCOSC_LF and RCOSC_HF?
     */
    config Bool calibrateRCOSC = true;

    /*!
     *  @_nodoc
     *   Wakeup Clock object's function.  Default is Power_defaultClockFunc.
     */
    config Clock.FuncPtr clockFunc = Power.defaultClockFunc;

    /*!
     *  @_nodoc
     *   LF Clock object's function.  Default is Power_lfClockFunc.
     */
    config Clock.FuncPtr lfClockFunc = Power.LF_clockFunc;

    /*!
     *  @_nodoc
     *  XOSC_HF Clock object's function.
     */
    config Clock.FuncPtr xoscClockFunc = Power.XOSC_HF_clockFunc;

    /*! Policy function.  Default is Power_doWFI. */
    metaonly config FuncPtr policyFunc = Power.doWFI;

    /*!
     * @_nodoc
     *  Notify trap function.  Default is Power.defaultNotifyTrapFunc.
     */
    config FuncPtr notifyTrapFunc = Power.defaultNotifyTrapFunc;

    // module functions

    /*!
     *  ======== standbyPolicy ========
     *  A power policy function that will transition the device into the
     *  Power_STANDBY sleep state during CPU idle time.
     *
     *  This is an agressive power policy function that will consider active
     *  constraints, sleep transition latencies, and the next expected wakeup,
     *  and automatically transition the device into the deepest sleep state
     *  possible.
     *
     *  The first goal is to enter Power_STANDBY; if that is not appropriate
     *  given current conditions (e.g., the sleep transition latency is
     *  greater than the time until the next scheduled Clock event), then
     *  the secondary goal is the IDLE_PD state; if that is disallowed (e.g.,
     *  if the Power_IDLE_PD_DISALLOW constraint is declared), then the policy
     *  will fallback and simply invoke wait-for-interrupt, to clock
     *  gate the CPU until the next interrupt.
     *
     *  This policy is enabled and selected via the following application
     *  configuration steps:
     *
     *  @p(code)
     *  Power.idle = true;
     *  Power.policyFunc = Power.standbyPolicy;
     *  @p
     */
    Void standbyPolicy();

    /*!
     * @_nodoc
     *  ======== defaultClockFunc ========
     *  Function for the Clock object dedicated for scheduling wakeups.
     */
    Void defaultClockFunc(UArg arg);

    /*!
     * @_nodoc
     *  ======== defaultNotifyTrapFunc ========
     *  The default function to be called when a notification client responds
     *  with any value other than Power_NOTIFYDONE.
     *
     *  The default behavior of this function is to spin in an infinite loop.
     */
    Void defaultNotifyTrapFunc();

    /*!
     * @_nodoc
     *  ======== LF_clockFunc ========
     *  Function for the Clock object dedicated for disabling the LF clock
     *  qualifiers.
     */
    Void LF_clockFunc(UArg arg);

    /*!
     *  @_nodoc
     *  ======== XOSC_HF_xoscClockFunc ========
     *  Function for the Clock object dedicated for XOSC_HF switching.
     */
    Void XOSC_HF_clockFunc(UArg arg);

    /*!
     *  @_nodoc
     *  ======== initateCalibration ========
     *  Initiates RCOSC_LF and RCOSC_HF calibration.
     */
    Bool initiateCalibration();

    /*!
     *  @_nodoc
     *  ======== startFirstMeasurement ========
     *  Start the first RCOSC calibration measurement
     */
    Void startFirstMeasurement();

    /*!
     *  @_nodoc
     *  ======== auxISR ========
     *  ISR for AUX combined interrupt for TDC (for RCOSC calibration)
     */
    Void auxISR(UArg arg);

    /*!
     *  ======== getClockHandle ========
     *  Get the handle of the Clock object created for scheduling wakeups.
     */
    Clock.Handle getClockHandle();

    /*!
     *  ======== doWFI ========
     *  A power policy function that invokes wait-for-interrupt (WFI)
     *  during idle time.
     *
     *  This is the most basic power policy function, which will invoke
     *  WFI upon entry to the Idle loop.  This is the default policy function
     *  that is invoked when CPU idling is enabled with the {@link #idle idle}
     *  configuration parameter:
     *
     *  @p(code)
     *  Power.idle = true;
     *  @p
     *
     */
    Void doWFI();

    /*!
     *  ======== getConstraintInfo ========
     *  Get a bitmask representing the aggregate of constraints that have been
     *  declared.
     *
     *  This function returns a bitmask indicating the constraints that are
     *  currently declared to the Power module (via previous calls to
     *  {@link #setConstraint Power_setConstraint()}).  For each constraint
     *  (of type {@link #Constraint Power_Constraint}) that is currently
     *  declared, the corresponding bit in the bitmask will be set.  For
     *  example, if two clients have declared Power_SB_DISALLOW, and one has
     *  declared Power_SB_VIMS_CACHE_RETAIN, then the returned bitmask
     *  will have the value: Power_SB_DISALLOW | Power_SB_VIMS_CACHE_RETAIN
     *
     *  @b(returns)     A bitmask representing all currently declared
     *                  constraints
     */
    UInt32 getConstraintInfo();

    /*!
     *  ======== getDependencyCount ========
     *  Get the count of dependencies that are currently declared upon a
     *  resource.
     *
     *  This function returns the number of dependencies that are currently
     *  declared upon a resource.
     *
     *  @param(resourceID)  The resource identifier, as specified in the
     *                      device-specific header file
     *
     *  @b(returns)     The number of dependencies currently declared upon this
     *                  resourceID
     */
    UInt32 getDependencyCount(UInt8 resourceID);

    /*!
     *  ======== getTransitionState ========
     *  Get the current Power module transition state.
     *
     *  @b(returns)     The current transition state, of type
     *                   {@link #TransitionState Power_TransitionState}
     */
    TransitionState getTransitionState();

    /*!
     *  ======== getTicksUntilWakeup ========
     *  Get the number of system ticks until the next scheduled wakeup event
     *
     *  @b(returns)     The number of system ticks (i.e.,
     *                  {@link ti.sysbios.knl.Clock Clock} module) ticks
     *                  until the next scheduled wakeup
     */
    UInt32 getTicksUntilWakeup();

    /*!
     *  ======== getTransitionLatency ========
     *  Get the minimal transition latency for a sleep state, in units of
     *  system Clock ticks.
     *
     *  Note that the reported latency is the device transition latency for
     *  the specified sleep state.  This latency value does not include any
     *  additional software induced latency, for example, that due to
     *  notifications of the power transition to drivers.
     *
     *  @param(sleepState)  The {@link #SleepState sleepState} whose transition
     *  latency is to be reported (e.g., Power_STANDBY)
     *
     *  @param(type)  The {@link #LatencyType type} of latency to be
     *  reported
     *
     *  @b(returns)     The transition latency, in units of
     *                  {@link ti.sysbios.knl.Clock Clock} ticks.
     */
    UInt32 getTransitionLatency(SleepState sleepState, LatencyType type);

    /*!
     *  ======== getXoscStartupTime ========
     *  Get the estimated crystal oscillator startup time, in units of
     *  microseconds.
     *
     *  @param(timeUntilWakeupInMs)  The estimated time until the next wakeup
     *                  event, in units of milliseconds
     *
     *  @b(returns)     The estimated crystal oscillator startup latency, in
     *                  units of microseconds.
     *
     *  @a(Note)  The estimated time until wakeup is in units of milliseconds,
     *            but the the estimate crystal startup time is in microseconds.
     */
    UInt32 getXoscStartupTime(UInt32 timeUntilWakeupInMs);

    /*!
     *  @_nodoc
     *  ======== isStableXOSC_HF ========
     *  Check if XOSC_HF has stabilized.
     *
     *  @b(returns)     TRUE if XOSC_HF has stabilized, FALSE if hasn't
     *                  stabilized yet
     */
    Bool isStableXOSC_HF();

    /*!
     *  ======== releaseConstraint ========
     *  Release a constraint that was previously declared with {@link
     *  #setConstraint Power_setConstraint()}.
     *
     *  This function releases a constraint that was previously declared with
     *  {@link #setConstraint Power_setConstraint()}.  For example, if a device
     *  driver is starting a transaction and wants to prohibit activation of
     *  the Power_STANDBY sleep mode, it uses {@link #setConstraint
     *  Power_setConstraint} to declare the constraint, before starting
     *  the transaction.  When the transaction completes, the driver calls
     *  this function to release the constraint, and allow the power manager
     *  to once again transition into the Power_STANDBY sleep mode, if
     *  appropriate.
     *
     *  @param(constraint)  The constraint being released, of type {@link
     *                  #Constraint Power_Constraint}
     *
     *  @a(constraints)
     *  Power_releaseConstraint() must not be called more than
     *  Power_setConstraint() has been called for the same constraint.  If
     *  asserts are enabled, one will be generated in this case.
     */
    Void releaseConstraint(Constraint constraint);

    /*!
     *  ======== releaseDependency ========
     *  Release a dependency that had been previously declared with {@link
     *  #setDependency Power_setDependency()}.
     *
     *  This function is the companion to {@link #setDependency
     *  Power_setDependency()}.  If this release is for the last outstanding
     *  dependency upon the resource, the resource will be de-activated as part
     *  of this function call.
     *
     *  @param(resourceID)  The resource identifier, as specified in the
     *                      device-specific header file
     *
     *  @a(constraints)
     *  Power_releaseDependency() must not be called more than
     *  Power_setDependency() has been called for the same resource.  If
     *  asserts are enabled, one will be generated in this case.
     */
    Void releaseDependency(UInt8 resourceID);

    /*!
     *  ======== registerNotify ========
     *  Register a function to be called upon a specific power event.
     *
     *  This function registers a function to be called when a
     *  Power event occurs. Registrations and the corresponding notifications
     *  are processed in first-in-first-out (FIFO) order. The function
     *  registered must behave as described below.
     *
     *  The pNotifyObj parameter is a pointer to a pre-allocated, opaque
     *  object that will be used by Power to support the notification.  This
     *  object could be dynamically allocated, or declared as a global object.
     *  Power_registerNotify() will properly initialized the object's fields as
     *  appropriate; the caller just needs to provide a pointer to this
     *  pre-existing object.
     *
     *  The eventType parameter identifies the type of power event for which
     *  the notify function being registered is to be called.  This parameter
     *  is treated as a bitmask, so multiple event types can be registered for
     *  a common function.  The event types are enumerated as {@link #Event
     *  Power_Event}.  For example, to call the specified function when
     *  both the entering standby and the awake from standby events occur,
     *  eventType should be specified as:
     *  (Power_ENTERING_STANDBY | Power_AWAKE_STANDBY)
     *
     *  The clientArg parameter is an arbitrary argument to be passed to the
     *  client upon notification. This argument may allow one notify function
     *  to be used by multiple instances of a driver (that is, the clientArg
     *  can be used to identify the instance of the driver that is being
     *  notified).
     *
     *  The notifyFxn parameter specifies the function to call when the
     *  specified Power event occurs. The notifyFxn must implement
     *  the following signature:
     *
     *      status = notifyFxn(eventType, clientArg);
     *
     *  Where: eventType (type of {@link #Event Power_Event}) is the event
     *  being signalled, and clientArg is the abitrary argument specified
     *  by the client at registration.  Note that multipe types of events
     *  can be specified when registering the notification callback function,
     *  but when the callback function is actually called by Power, only a
     *  single eventType will be specified for the callback (i.e., the current
     *  event).
     *
     *  The client notification function being registered must return one of the
     *  following constants as a status value, of type {@link #NotifyResponse
     *  Power_NotifyResponse}: Power_NOTIFYDONE if the client processed the
     *  notification function successfully, or Power_NOTIFYERROR if the
     *  notification cannot be processed (e.g., if an internal client error
     *  occurs).
     *
     *  @param(pNotifyObj)  The notify object to be used for queuing
     *                      of the notification callback function
     *
     *  @param(eventTypes)  The type or types of {@link #Event Power_Event} to
     *                      be notified of
     *
     *  @param(notifyFxn)  The callback function Power should call to notify
     *                     the client of the event
     *
     *  @param(clientArg)  A client-defined argument to pass with the notifyFxn
     *                     call
     *
     *  @param(arg)        For future use, should be specified as:
     *                     NULL
     *
     *  @b(returns)     Power_SOK if the registration succeeded
     *
     *  @b(returns)     Power_EINVALIDPOINTER if either pNotifyObj or notifyFxn
     *                  were NULL
     *
     */
    Status registerNotify(NotifyObj * pNotifyObj, UInt32 eventTypes,
        Fxn notifyFxn, UArg clientArg, UArg arg);

    /*!
     *  ======== setConstraint ========
     *  Declare an operational constraint.
     *
     *  Before taking certain actions, Power checks to see if the requested
     *  action would conflict with a client-declared constraint. If the
     *  action does conflict, Power will not proceed with the request.
     *  This is the function that allows clients to declare their constraints
     *  with Power.
     *
     *  It is critical that clients call {@link #releaseConstraint
     *  Power_releaseConstraint()} when operational constraints no longer
     *  exists. Otherwise, Power may be left unnecessarily restricted from
     *  activating power savings.
     *
     *  @param(constraint)  The constraint being declared, of type {@link
     *                  #Constraint Power_Constraint}
     */
    Void setConstraint(Constraint constraint);

    /*!
     *  ======== setDependency ========
     *  Declare a dependency upon a power-manageable resource.
     *
     *  This function declares a dependency upon a resource.  If the resource
     *  had been inactive, it will be activated during this function call.
     *
     *  This function is the companion to {@link #releaseDependency
     *  Power_releaseDependency()}.
     *
     *  @param(resourceID)  The resource identifier, as specified in the
     *                      device-specific header file (for example,
     *                      PERIPH_UART0, as defined in PowerCC2650.h)
     *  @a(Note)
     *  When a dependency is declared upon a resource for the first time, the
     *  resource will be activated before the call to Power_setDependency()
     *  returns to the caller.  There is only one exception to this rule:
     *  the XOSC_HF resource for CC26xx.  The latency to startup the crystal
     *  is much longer than that for other resources. To avoid blocking in
     *  Power_setDependency() for this duration, for the XOSC_HF resource
     *  only, Power_setDependency() will return quickly, after initiating the
     *  activation of XOSC_HF.  Once the switchover to the XOSC_HF
     *  happens, notifications will be sent to clients registered for the
     *  Power_XOSC_HF_SWITCHED event.
     *
     *  Power_setDependency() will return a Boolean status to indicate if
     *  the resource is active as the function is initiating a return to the
     *  caller.  For all resources other than XOSC_HF, the return status will
     *  always be TRUE, because the resource will be active before the return
     *  to the caller.  For XOSC_HF, the return status may be TRUE if the
     *  XOSC_HF is already active.  For example, if XOSC_HF was already
     *  activated with an earlier call to Power_setDependency(XOSC_HF).
     *  If the return status is FALSE, this indicates to the caller that
     *  XOSC_HF hadn't been switched by the time the function initiated a
     *  return, and that the caller should wait for the Power_XOSC_HF_SWITCHED
     *  event notification before proceeding to the work that requires XOSC_HF
     *  to be active.
     *
     *  The recommended sequence for a caller to activate XOSC_HF is:
     *
     *
     *      1) Caller registers for a Power_XOSC_HF_SWITCHED notification.
     *         For example, the specified callback function might set a global
     *         flag, which the caller can check to see if the notification
     *         occurred.
     *
     *      2) Caller invokes: status = Power_setDependency(XOSC_HF);
     *
     *
     *      3) Caller checks status to see if XOSC_HF is active yet
     *
     *
     *          - If status is TRUE, XOSC_HF is active, caller should proceed,
     *            and NOT wait for a Power_XOSC_HF_SWITCHED notification
     *
     *          - Else, XOSC_HF wasn't activated yet, caller needs to wait
     *            until the Power_XOSC_HF_SWITCHED event has been signaled.
     *            Note that depending upon timing, the event may have been
     *            signaled between the time the return status was determined
     *            within Power_setDependency() and the time the caller
     *            checks the return status.  The key point is that, when
     *            the return status is FALSE, the Power_XOSC_HF_SWITCHED
     *            notification WILL be sent (once the XOSC_HF is switched),
     *            and the caller should wait for this notification before
     *            proceeding with any work that requires XOSC_HF to be active.
     *
     *  @b(returns)     TRUE if resource is active
     *
     *  @b(returns)     FALSE if resource was not yet active when the function
     *                  return was initiated
     *
     *  @a(constraints)
     *  For the XOSC_HF resource (described above), the caller MUST wait for
     *  XOSC_HF activation to complete before calling
     *  Power_releaseDependency(XOSC_HF).
     */
    Bool setDependency(UInt8 resourceID);

    /*!
     *  ======== shutdown ========
     *  Put the device into the SHUTDOWN state.
     *
     *  This function will transition the device into the SHUTDOWN state.
     *  Before the actual device transtion is initiated, notifications will
     *  be sent to any clients who've registered for the
     *  Power_ENTERING_SHUTDOWN event (via {@link #registerNotify
     *  Power_registerNotify()}).  Once notifications have been completed, the
     *  device shutdown will commence.
     *
     *  If the device is successfully transitioned to SHUTDOWN, this function
     *  call will never return.  Upon wakeup, the device and application will
     *  be rebooted.  If the transition to SHUTDOWN is not successful, one of
     *  the error codes listed above will be returned.
     *
     *  @param(arg)     Reserved for future use; currently unused
     *
     *  @b(returns)     Power_EBUSY if Power is currently busy with another
     *                  transition
     *
     *  @b(returns)     Power_ECHANGE_NOT_ALLOWED   if SHUTDOWN is disallowed
     *                  due to the Power_SD_DISALLOW constraint having been
     *                  declared
     *
     *  @b(returns)     Power_EFAIL   if a notification error occurred
     *
     *  @a(Note)
     *  This function will not prepare I/O pins for the device
     *  shutdown state.  The application needs to do the appropriate
     *  I/O preparation before calling this API.
     */
    Status shutdown(UArg arg);

    /*!
     *  ======== sleep ========
     *  Put the CPU into a sleep state.
     *
     *  This function transitions the device into and out of a sleep state.
     *  The function returns to the caller when the device is awake again.
     *
     *  @param(sleepState)  The sleep state to be entered, of type
     *                      {@link #sleepState Power_sleepState}
     *
     *  @param(arg0)    Reserved for future use; currently unused
     *
     *  @param(arg1)    Reserved for future use; currently unused
     *
     *  @b(returns)     Power_SOK upon success, the device is awake
     *
     *  @b(returns)     Power_EBUSY if Power is currently busy with another
     *                  transition
     *
     *  @b(returns)     Power_EFAIL if a general failure occurred and the
     *                  device was not transitioned into sleep
     *
     *  @a(Note)
     *  Power_sleep() is intended to be called from a Power Policy
     *  implementation function.  This API is not normally called directly
     *  by an application.
     *
     *  @a(constraints)
     *  Power_sleep() does not check the constraints that might prohibit a
     *  sleep transition.  The caller of Power_sleep() must validate the
     *  transition versus active constraints, prior to calling Power_sleep().
     */
    Status sleep(SleepState sleepState, UArg arg0, UArg arg1);

    /*!
     *  @_nodoc
     *  ======== switchXOSC_HF ========
     *  Switch to enable XOSC_HF.
     */
    Void switchXOSC_HF();

    /*!
     *  ======== unregisterNotify ========
     *  Unregister for Power event notifications.
     *
     *  This function unregisters for event notifications that were previously
     *  registered with {@link #registerNotify Power_registerNotify()}.
     *
     *  @param(pNotifyObj)  The notification object that was previously
     *                      used with {@link #registerNotify
     *                      Power_registerNotify()} to register for
     *                      event notifications
     */
    Void unregisterNotify(NotifyObj * pNotifyObj);


    /*! @_nodoc */
    @XmlDtd
    metaonly struct ModuleView {
        String    ConstraintsMask;
    };

    /*! @_nodoc */
    @Facet
    metaonly config ViewInfo.Instance rovViewInfo =
        ViewInfo.create({
            viewMap: [
            [
                'Module',
                {
                    type: ViewInfo.MODULE,
                    viewInitFxn: 'viewInitModule',
                    structName: 'ModuleView'
                }
            ],
            ]
        });

    /*!
     *  @_nodoc
     *  ======== idleCPU ========
     *  Function used to automatically idle the CPU in the Idle loop.
     *
     *  When the 'idle' configuration parameter is set, this function will
     *  be added to the list of Idle loop functions.  When called from the
     *  Idle loop, it will invoke the wait for interrupt (WFI) instruction,
     *  to idle the CPU until the next interrupt occurs.
     */
    Void idleCPU();

internal:

    /*
     *  ======== notify ========
     */
    Status notify(Event eventType);

    /*
     *  ======== serviceNotifyQ ========
     */
    NotifyResponse serviceNotifyQ(Event eventType);

    /*
     *  ======== Module_State ========
     */
    struct Module_State {
        UInt32 constraintsMask;
        Queue.Object notifyQ;       /* event notification queue */
        Clock.Object clockObj;      /* Clock object for scheduling wakeups */
        Clock.Object xoscClockObj;  /* Clock object for XOSC_HF switching */
        Clock.Object lfClockObj;    /* Clock object for LF clock check */
        TransitionState state;
        Bool xoscPending;           /* is XOSC_HF activation in progress? */
    }
}
