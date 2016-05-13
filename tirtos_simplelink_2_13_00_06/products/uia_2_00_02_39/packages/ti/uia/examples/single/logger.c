/*
 *  ======== logger.c ========
 *  Example showing use of logs.
 */

#include <xdc/std.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Timestamp.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/uia/sysbios/LoggerStreamer.h>
#include <ti/uia/runtime/LogUC.h>

#ifdef xdc_target__isaCompatible_64P
    #include <c6x.h>
#else
    #define DNUM 0
#endif

#include <xdc/cfg/global.h>

volatile Int count = 0;
Char buffer[3][1024];

/* Called during XDC startup...well before main() */
Ptr prime()
{
    LoggerStreamer_initBuffer(buffer[0], DNUM);
    LoggerStreamer_initBuffer(buffer[1], DNUM);
    LoggerStreamer_initBuffer(buffer[2], DNUM);

    return ((Ptr)(buffer[0]));
}

/* Called when LoggerStream is full */
Ptr exchange(Ptr *full)
{
    count++;
    return ((Ptr *)buffer[count % 3]);
}

/*
 *  ======== tsk0Fxn ========
 *  Statically created task
 */
Void tsk0Fxn(UArg arg0, UArg arg1)
{
    Int num = 0;

    /* To demonstrate the exchange function */
    while (TRUE) {
        Log_printUC1(Diags_USER1, "Num = %d", num++);
        Task_sleep(1);
    }
}

/*
 *  ======== main ========
 */
Int main(Int argc, Char* argv[])
{
    register UInt32 t1, t2;

#ifdef xdc_target__isaCompatible_64P
    t1 = TSCL;
    Log_writeUC0(LoggerStreamer_L_test);
    t2 = TSCL;
    System_printf("Log_print0 (using TSCL) = %d\n", t2 - t1 - 1);
#else
    register Int overhead;
    t1 = Timestamp_get32();
    t2 = Timestamp_get32();
    overhead = t2 - t1;
    t1 = Timestamp_get32();
    Log_writeUC0(LoggerStreamer_L_test);
    t2 = Timestamp_get32();
    System_printf("Log_print0 = %d\n", (t2 - t1 - overhead));
#endif

    BIOS_start();

    return (0);
}
