/*
 * Copyright (c) 2012-2014, Texas Instruments Incorporated
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
 *  ======== LoggerSM.c ========
 */
#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Text.h>
#include <xdc/runtime/Gate.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Timestamp.h>

#include <ti/uia/runtime/EventHdr.h>

#ifdef xdc_target__isaCompatible_64P
#include <c6x.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "package/internal/LoggerSM.xdc.h"

/*
 * Conditionally set the format string depending on the width of data
 * primitives. This should be removed once System_printf supports
 * 32-bit format strings
 */
#if (xdc_target__bitsPerChar * xdc_target__sizeof_Int) == 32
#define SERIALTIME "#:%05u T:%0.8x%0.8x M:"
#define SERIAL "#:%05u M:"
#define SERIALTIMESIZE 29
#define SERIALSIZE 10
#define STRINGFLAG " S:"
#define STRINGFLAGSIZE 3
#elif (xdc_target__bitsPerChar * xdc_target__sizeof_Long) == 32
#define SERIALTIME "#:%05lu T:%0.8lx%0.8lx M:"
#define SERIAL "#:%05lu M:"
#define SERIALTIMESIZE 29
#define SERIALSIZE 10
#define STRINGFLAG " S:"
#define STRINGFLAGSIZE 3
#else
#error xdc.runtime.Log does not support this target.
#endif

#define TIMESTAMP                     LoggerSM_isTimestampEnabled
#define BITS8_IN_EVENTHDR           8
#define BITS8_IN_TIMESTAMP          ((TIMESTAMP & 1) << 3)
#define BITS8_IN_WRITEMEMORY        24
#define BITS8_IN_NUM_ARGS_0         0
#define BITS8_IN_NUM_ARGS_1         4
#define BITS8_IN_NUM_ARGS_2         8
#define BITS8_IN_NUM_ARGS_4         16
#define BITS8_IN_NUM_ARGS_8         32

#define BITS8_TO_BITS32(bits8)      ((bits8) / 4)
#define BITS32_TO_BITS8(bits32)     ((bits32) * 4)
#define MAU_TO_BITS32(mau)          ((mau) / sizeof(Bits32))
#define MAU_TO_BITS32_ROUND_UP(mau) (MAU_TO_BITS32((mau) + sizeof(Bits32) - 1))
#define BITS32_TO_MAU(bits32)       ((bits32) * sizeof(Bits32))
#define BITS8_TO_MAU(bits8)         (BITS32_TO_MAU(BITS8_TO_BITS32(bits8)))
#define MAU_TO_BITS8(mau)           (BITS32_TO_BITS8(MAU_TO_BITS32(mau)))

#define TEMPARRAYSIZE 220

/*
 *  ======== fillInTimestamp =========
 */
static inline UInt32 *fillInTimestamp(UInt32 *writePtr)
{
#ifndef xdc_target__isaCompatible_64P
    Types_Timestamp64 tstamp;

    Timestamp_get64(&tstamp);
    *(writePtr++) = tstamp.lo;
    *(writePtr++) = tstamp.hi;
#else
    *(writePtr++) = TSCL;
    *(writePtr++) = TSCH;

#endif
    return (writePtr);
}

/*
 *  ======== fillInEventHdr =========
 */
static inline UInt32 *fillInEventHdr(UInt32 *writePtr, SizeT numBits8ToWrite,
                                   Bits32 serial)
{
    if (TIMESTAMP) {
        *(writePtr++) = EventHdr_genEventHdrWord1(numBits8ToWrite, serial,
                           EventHdr_HdrType_EventWithTimestamp);
        writePtr = fillInTimestamp(writePtr);
    }
    else {
        *(writePtr++) = EventHdr_genEventHdrWord1(numBits8ToWrite, serial,
                           EventHdr_HdrType_Event);
    }
    return (writePtr);
}

/*
 *  ======== LoggerSM_getFreeSize =========
 */
static SizeT LoggerSM_getFreeSize()
{
    register Bits32 reader = (Bits32)LoggerSM_module->sharedObj->readPtr;
    register Bits32 writer = (Bits32)LoggerSM_module->sharedObj->writePtr;

    if (reader > writer) {
        return (reader - writer);
    }

    return (LoggerSM_module->sharedObj->bufferSizeMAU - (writer - reader));
}

/*
 *  ======== eraseRecords =========
 */
static Void eraseRecords(SizeT numMAUToWrite)
{
    SizeT freeSizeMAU = LoggerSM_getFreeSize();
    SizeT numMAUInEvent = 0;
    Bits32 endPtr = (Bits32)LoggerSM_module->sharedObj->endPtr;
    volatile Char *readPtr = LoggerSM_module->sharedObj->readPtr;

    while (freeSizeMAU <= numMAUToWrite) {
        /*
         *  - Get the size of the event record
         *  - Update the running count
         *  - adjust the temp read pointer
         */
        numMAUInEvent = EventHdr_getLength(*(Int32 *)readPtr);
        freeSizeMAU += numMAUInEvent;
        readPtr = (Ptr)(readPtr + numMAUInEvent);

        /* check for wrap condition */
        if ((Bits32)readPtr >= endPtr) {
            readPtr = (Ptr)(LoggerSM_module->sharedObj->buffer +
                              ((Bits32)readPtr - endPtr));
        }
    }
    LoggerSM_module->sharedObj->readPtr = readPtr;
}

/*
 *  ======== LoggerSM_updateWritePtrByValue ========
 *  updates the current write pointer, wrapping if necessary
 */
static inline Void LoggerSM_updateWritePtrByValue(Ptr newWrPtr)
{
    Bits32 temp;
    Char *endPtr = LoggerSM_module->sharedObj->endPtr;

    /* Did not write in the padded area. Just set the new writePtr and return */
    if ((Char *)newWrPtr < endPtr) {
        LoggerSM_module->sharedObj->writePtr = newWrPtr;
        return;
    }

    /*
     *  Determine how much is in the padded region. Move that data
     *  back to the start.
     */
    temp = (Bits32)newWrPtr - (Bits32)endPtr;
    if (temp > 0) {
        memcpy(LoggerSM_module->sharedObj->buffer, endPtr, temp);
    }

    /*
     *  NOTE: Must only update obj->writePtr after it is safe for the reader to
     *  read the data for the message from the buffer and update the readPtr.
     */
    LoggerSM_module->sharedObj->writePtr =
        LoggerSM_module->sharedObj->buffer + temp;
    return;
}

/*
 *  ======== LoggerSM_setPartitionId ========
 */
Void LoggerSM_setPartitionId(Int partitionId)
{
    LoggerSM_module->partitionId = partitionId;
}

/*
 *  ======== LoggerSM_setupBuffer ========
 */
Void LoggerSM_setupBuffer(Ptr sharedMemory, Bits32 sharedMemorySize)
{
    SizeT partitionSize = sharedMemorySize / LoggerSM_numPartitions;
    SizeT bufferSize;

    /* Adjust to be a multiple of 128 */
    partitionSize = partitionSize & (~127);

    Assert_isTrue(partitionSize > 256, NULL);

    /*
     * The actual buffer size
     * Note: 128 pad at the beginning for partition info
     *       128 pad at the end for easier writing of records
     *
     */
    bufferSize = partitionSize - 256;

    /* todo Make sure each partition has a buffer that is a multiple of cacheline */
    LoggerSM_module->sharedObj = (LoggerSM_SharedObj *)((Char *)sharedMemory +
                                  partitionSize * LoggerSM_module->partitionId);
    LoggerSM_module->sharedObj->buffer =
        (Char *)((Char *)(LoggerSM_module->sharedObj) + 128);
    LoggerSM_module->sharedObj->version = LoggerSM_VERSION;
    LoggerSM_module->sharedObj->numPartitions = LoggerSM_numPartitions;
    LoggerSM_module->sharedObj->readPtr = LoggerSM_module->sharedObj->buffer;
    LoggerSM_module->sharedObj->writePtr = LoggerSM_module->sharedObj->buffer;
    LoggerSM_module->sharedObj->endPtr =
        (Char *)((Char *)(LoggerSM_module->sharedObj->buffer) + bufferSize);
    LoggerSM_module->sharedObj->bufferSizeMAU = bufferSize;
    LoggerSM_module->sharedObj->droppedEvents = 0;
    LoggerSM_module->sharedObj->decode = LoggerSM_decode;
    LoggerSM_module->sharedObj->overwrite = LoggerSM_overwrite;
    LoggerSM_module->sharedObj->moduleId = ti_uia_runtime_LoggerSM_Module_id();
    LoggerSM_module->sharedObj->instanceId = 1;
    LoggerSM_module->sharedObj->headerTag = 0x14011938;
}


/*
 *  ======== LoggerSM_setSharedMemory ========
 */
Bool LoggerSM_setSharedMemory(Ptr sharedMemory, Bits32 sharedMemorySize)
{
    /* Make sure buffer was not already set */
    if (LoggerSM_module->sharedObj != NULL) {
        return (FALSE);
    }

    LoggerSM_setupBuffer(sharedMemory, sharedMemorySize);

    return (TRUE);
}

/*
 *  ======== LoggerSM_Module_startup ========
 */
Int LoggerSM_Module_startup(Int phase)
{
    /*
     *  Make sure there is a buffer to setup. There is none when
     *  LoggerSM.sharedMemorySize is set to 0.
     */
    if (LoggerSM_sharedMemorySize != 0) {
        LoggerSM_setupBuffer(LoggerSM_module->sharedBuffer, LoggerSM_sharedMemorySize);
    }

    return (Startup_DONE);
}

/*
 *  ======== LoggerSM_enable ========
 */
Bool LoggerSM_enable(LoggerSM_Object *obj)
{
    Bool prev;
    IArg key;

    key = Gate_enterSystem();

    prev = LoggerSM_module->enabled;
    LoggerSM_module->enabled = TRUE;

    Gate_leaveSystem(key);

    return (prev);
}

/*
 *  ======== LoggerSM_disable ========
 */
Bool LoggerSM_disable(LoggerSM_Object *obj)
{
    Bool prev;
    IArg key;

    key = Gate_enterSystem();

    prev = LoggerSM_module->enabled;
    LoggerSM_module->enabled = FALSE;

    Gate_leaveSystem(key);

    return (prev);
}

/*
 *  ======== LoggerSM_setFilterLevel ========
 *  Sets the filter level for the given diags level.
 *
 *  LoggerSM maintains a separate filter level for every diags category.
 *  This is accomplished by maintaining three masks, one for each of the levels
 *  1 - 3, wich store the diags categories which are currently at that level.
 *  There is no mask for level4; if the diags category is not found in levels
 *  1-3, it is  assumed that the filtering level is level4.
 *
 *  This API is an instance function per the IFilterLogger interface, but
 *  LoggerSM only maintains module-wide filter levels.
 *
 *  TODO - Should this be conditional on the 'filterByLevel' config?
 */
Void LoggerSM_setFilterLevel(LoggerSM_Object *obj,
                                  xdc_runtime_Diags_Mask mask,
                                  xdc_runtime_Diags_EventLevel filterLevel)
{
    /*
     * First, remove the bits in 'mask' from all of the current 'level' masks.
     * Use level = (~(mask & level) & level) to remove 'mask' bits from all
     * 'level's.
     *    1. AND mask and level to get set of bits that appear in both
     *    2. Take the inverse of this set and AND it with 'level' to disable
     *       any bits which appear in 'mask'.
     */
    LoggerSM_module->level1 = ~(LoggerSM_module->level1 & mask) &
                               LoggerSM_module->level1;
    LoggerSM_module->level2 = ~(LoggerSM_module->level2 & mask) &
                               LoggerSM_module->level2;
    LoggerSM_module->level3 = ~(LoggerSM_module->level3 & mask) &
                               LoggerSM_module->level3;

    /* Enable the bits specified in 'mask' in the appropriate level. */
    switch (filterLevel) {
        case Diags_LEVEL1:
            LoggerSM_module->level1 |= mask;
            break;
        case Diags_LEVEL2:
            LoggerSM_module->level2 |= mask;
            break;
        case Diags_LEVEL3:
            LoggerSM_module->level3 |= mask;
            break;
        case Diags_LEVEL4:
            break;
        default: {
            /* Raise an error that a bad filter level was received. */
            Error_Block eb;
            Error_init(&eb);
            Error_raise(&eb, LoggerSM_E_badLevel, filterLevel, 0);
            break;
        }
    }
}

/*
 *  ======== LoggerSM_getFilterLevel ========
 *  Returns the mask of diags categories which are set to the specified filter
 *  level.
 *
 *  This API is an instance function per the IFilterLogger interface, but
 *  LoggerSM only maintains module-wide filter levels.
 */
Diags_Mask LoggerSM_getFilterLevel(LoggerSM_Object *obj,
                                    xdc_runtime_Diags_EventLevel level)
{
    /* Return the mask of diags categories associated with 'level'. */
    switch (level) {
        case Diags_LEVEL1:
            return (LoggerSM_module->level1);
        case Diags_LEVEL2:
            return (LoggerSM_module->level2);
        case Diags_LEVEL3:
            return (LoggerSM_module->level3);
        case Diags_LEVEL4:
            /*
             * Return the inverse of all the bits set in levels 1 - 3,
             * and only return those bits which belong to the set of
             * logging categories (excludes asserts and the two level bits).
             */
            return (~(LoggerSM_module->level1 |
                      LoggerSM_module->level2 |
                      LoggerSM_module->level3) &
                     xdc_runtime_Diags_ALL_LOGGING);
        default: {
            /* Raise an error to report the bad filter level. */
            Error_Block eb;
            Error_init(&eb);
            Error_raise(&eb, LoggerSM_E_badLevel, level, 0);
            return (0);
        }
    }
}

/*
 *  ======== LoggerSM_filterOutEvent ========
 *  Returns TRUE if the event should be filtered out.
 *  TODO - Might be more clearly written as 'passesFilter'?
 */
Bool LoggerSM_filterOutEvent(xdc_runtime_Diags_Mask mask)
{
    /*
     * If filtering for the event's diags category is currently
     * set to level1...
     */
    if (LoggerSM_module->level1 & mask) {
        /* If the event is lower than level1, filter it out. */
        return(Diags_compareLevels(Diags_getLevel(mask), Diags_LEVEL1));
    }
    /*
     * If filtering for the event's diags category is currently
     * set to level2...
     */
    else if (LoggerSM_module->level2 & mask) {
        /* If the event is lower than level2, filter it out. */
        return(Diags_compareLevels(Diags_getLevel(mask), Diags_LEVEL2));
    }
    /*
     * If filtering for the event's diags category is currently
     * set to level3...
     */
    else if (LoggerSM_module->level3 & mask) {
        /* If the event is lower than level3, filter it out. */
        return(xdc_runtime_Diags_compareLevels(xdc_runtime_Diags_getLevel(mask),
                                               xdc_runtime_Diags_LEVEL3));
    }
    /*
     * Otherwise, the filter level must be level4, which means that events of
     * all levels should be logged.
     */
    else {
        return (FALSE);
    }
}

/*
 *  ======== LoggerSM_reset ========
 */
Void LoggerSM_reset(LoggerSM_Object *obj)
{
    /* Do not do any work if there is no shared object yet */
    if (LoggerSM_module->sharedObj == NULL) {
        return;
    }

    // assert that logger is disabled?
    // TODO memset buffer?
    LoggerSM_module->sharedObj->readPtr = LoggerSM_module->sharedObj->buffer;
    LoggerSM_module->sharedObj->writePtr = LoggerSM_module->sharedObj->buffer;
    LoggerSM_module->serial        = 1;

    LoggerSM_module->enabled = TRUE;
}

/*
 *  ======== LoggerSM_overwriteDecoded ========
 */
Void LoggerSM_overwriteDecoded(Char *tempArray, SizeT len)
{
    Char *buffer = LoggerSM_module->sharedObj->buffer;
    Char *writePtr = LoggerSM_module->sharedObj->writePtr;
    Char *endPtr = LoggerSM_module->sharedObj->endPtr;
    SizeT end;

    /*
     * Case 1: overwrite does not wrap
     * Case 2: overwrite does wrap
     */
     if (len + (Bits32)writePtr < (Bits32)endPtr) {
         memcpy(writePtr, tempArray, len);
         writePtr += len;
     }
     else {
         end = (SizeT)((Bits32)endPtr - (Bits32)writePtr);
         memcpy(writePtr, tempArray, end);
         memcpy(buffer, &(tempArray[end]), len - end);
         writePtr = buffer + (len - end);
     }

     if ((writePtr + 1) == endPtr) {
         LoggerSM_module->sharedObj->readPtr = buffer;
     }
     else {
         LoggerSM_module->sharedObj->readPtr = writePtr + 1;
     }
     LoggerSM_module->sharedObj->writePtr = writePtr;
}

/*
 *  ======== LoggerSM_write =========
 */
Void LoggerSM_write(LoggerSM_Object *obj, Log_Event evt,
        Types_ModuleId mid, IArg a1, IArg a2, IArg a3, IArg a4,
        IArg a5, IArg a6, IArg a7, IArg a8)
{
    Bits32 hi, lo;
    Char *tempStr;
    volatile Char *readPtr;
    Char *writePtr;
    Char tempArray[TEMPARRAYSIZE];
    SizeT len = 0;
    IArg key;
    Text_RopeId rope;
    String fmt;
    SizeT end;
    Bits16 serial;

    key = Gate_enterSystem();
    serial = LoggerSM_module->serial++;

    /* If disabled, simply increment the serial counter */
    if (LoggerSM_module->enabled == FALSE) {
        LoggerSM_module->sharedObj->droppedEvents++;
        Gate_leaveSystem(key);
        return;
    }
    Gate_leaveSystem(key);

    if (LoggerSM_isTimestampEnabled == TRUE) {
#ifndef xdc_target__isaCompatible_64P
        Types_Timestamp64 tstamp;

        Timestamp_get64(&tstamp);
        lo = tstamp.lo;
        hi = tstamp.hi;
#else
        lo = TSCL;
        hi = TSCH;
#endif
        key = Gate_enterSystem();
        System_sprintf(tempArray, SERIALTIME, serial, hi, lo);
        Gate_leaveSystem(key);
        len = SERIALTIMESIZE;
    }
    else {
        key = Gate_enterSystem();
        System_sprintf(tempArray, SERIAL, serial);
        Gate_leaveSystem(key);
        len = SERIALSIZE;
    }

    /* pointer to memory after the serial num and timestamp */
    tempStr = &(tempArray[len]);

    /* Put in the module name */
    Text_putMod(mid, &tempStr, TEMPARRAYSIZE - len);

    /* Put the string delimitor in */
    System_sprintf(tempStr, STRINGFLAG);
    tempStr += STRINGFLAGSIZE;

    rope = Log_getEventId(evt);   /* the event id is the message rope */
    if (rope == 0) {
        /* Log_print() event */
        key = Gate_enterSystem();
        System_asprintf(tempStr, (String)iargToPtr(a1),
            a2, a3, a4, a5, a6, a7, a8,  0);
        Gate_leaveSystem(key);
    }
    else {
        fmt = Text_ropeText(rope);
        key = Gate_enterSystem();
        if (Text_isLoaded) {
            System_asprintf(tempStr, fmt, a1, a2, a3, a4, a5, a6, a7, a8);
        }
        else {
            System_asprintf(tempStr, "{evt: fmt=%p, args=[0x%x, 0x%x ...]}",
                fmt, a1, a2);
        }
        Gate_leaveSystem(key);
    }

    len = strlen(tempArray);
    tempArray[len] = '\n';
    len++;
    key = Gate_enterSystem();
    readPtr = LoggerSM_module->sharedObj->readPtr;
    writePtr = LoggerSM_module->sharedObj->writePtr;
    if (writePtr < readPtr) {
        if (len < (Bits32)readPtr - (Bits32)writePtr) {
            /* Case 1 */
            memcpy(writePtr, tempArray, len);
            LoggerSM_module->sharedObj->writePtr += len;
        }
        else if (LoggerSM_overwrite == TRUE) {
            LoggerSM_overwriteDecoded(tempArray, len);
        }
        else {
            LoggerSM_module->sharedObj->droppedEvents++;
        }
    }
    else {
        if (len < (LoggerSM_module->sharedObj->bufferSizeMAU -
            (SizeT)((Bits32)writePtr - (Bits32)readPtr))) {
            end = (SizeT)((Bits32)LoggerSM_module->sharedObj->endPtr - (Bits32)writePtr);
            if (end > len) {
                /* Case 2 */
                memcpy(writePtr, tempArray, len);
                LoggerSM_module->sharedObj->writePtr += len;
            }
            else {
                /* Case 3 */
                memcpy(writePtr, tempArray, end);
                memcpy(LoggerSM_module->sharedObj->buffer,
                    &(tempArray[end]), len - end);
                LoggerSM_module->sharedObj->writePtr =
                    LoggerSM_module->sharedObj->buffer + len - end;
            }
        }
        else if (LoggerSM_overwrite == TRUE) {
            LoggerSM_overwriteDecoded(tempArray, len);
        }
        else {
            LoggerSM_module->sharedObj->droppedEvents++;
        }
    }

    Gate_leaveSystem(key);
}

/*
 *  ======== write0 =========
 *  Log an event with 0 parameters and the calling address
 */
Void LoggerSM_write0(LoggerSM_Object *obj, Log_Event evt, Types_ModuleId mid)
{
    IArg key;
    Bits32 *myWrPtr;
    SizeT numBits8ToWrite;

    /* Do not do any work if there is no shared object yet */
    if (LoggerSM_module->sharedObj == NULL) {
        return;
    }

    if (LoggerSM_decode == TRUE) {
        LoggerSM_write(obj, evt, mid, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    else {
        /* Figure out big the write will be */
        numBits8ToWrite = BITS8_IN_EVENTHDR + BITS8_IN_NUM_ARGS_0 +
                                                            BITS8_IN_TIMESTAMP;

        key = Gate_enterSystem();
        if (LoggerSM_getFreeSize() <= BITS8_TO_MAU(numBits8ToWrite)) {
            if (LoggerSM_overwrite == FALSE) {
                LoggerSM_module->serial++;
                LoggerSM_module->sharedObj->droppedEvents++;
                Gate_leaveSystem(key);
                return;
            }
            else {
                eraseRecords(BITS8_TO_MAU(numBits8ToWrite));
            }
        }
        myWrPtr = (Bits32 *)fillInEventHdr((UInt32 *)(LoggerSM_module->sharedObj->writePtr),
                      numBits8ToWrite, LoggerSM_module->serial++);
        *(myWrPtr++) = ((evt) & 0xffff0000) | mid;
        LoggerSM_updateWritePtrByValue(myWrPtr);

        Gate_leaveSystem(key);
    }
}
/*
 *  ======== write1 =========
 *  Log an event with 1 parameter and the calling address
 */
Void LoggerSM_write1(LoggerSM_Object *obj, Log_Event evt, Types_ModuleId mid,
                     IArg a1)
{
    IArg key;
    Bits32 *myWrPtr;
    SizeT numBits8ToWrite;

    /* Do not do any work if there is no shared object yet */
    if (LoggerSM_module->sharedObj == NULL) {
        return;
    }

    if (LoggerSM_decode == TRUE) {
        LoggerSM_write(obj, evt, mid, a1, 0, 0, 0, 0, 0, 0, 0);
    }
    else {
        /* Figure out big the write will be */
        numBits8ToWrite = BITS8_IN_EVENTHDR + BITS8_IN_NUM_ARGS_1 +
                                                            BITS8_IN_TIMESTAMP;

        key = Gate_enterSystem();

        if (LoggerSM_getFreeSize() <= BITS8_TO_MAU(numBits8ToWrite)) {
            if (LoggerSM_overwrite == FALSE) {
                LoggerSM_module->serial++;
                LoggerSM_module->sharedObj->droppedEvents++;
                Gate_leaveSystem(key);
                return;
            }
            else {
                eraseRecords(BITS8_TO_MAU(numBits8ToWrite));
            }
        }

        myWrPtr = (Bits32 *)fillInEventHdr((UInt32 *)LoggerSM_module->sharedObj->writePtr,
                      numBits8ToWrite, LoggerSM_module->serial++);
        *(myWrPtr++) = ((evt) & 0xffff0000) | mid;
        *(myWrPtr++) = a1;
        LoggerSM_updateWritePtrByValue(myWrPtr);
        Gate_leaveSystem(key);
    }
}
/*
 *  ======== write2 =========
 *  Log an event with 2 parameter and the calling address
 */
Void LoggerSM_write2(LoggerSM_Object *obj, Log_Event evt, Types_ModuleId mid,
                     IArg a1, IArg a2)
{
    IArg key;
    Bits32 *myWrPtr;
    SizeT numBits8ToWrite;

    /* Do not do any work if there is no shared object yet */
    if (LoggerSM_module->sharedObj == NULL) {
        return;
    }

    if (LoggerSM_decode == TRUE) {
        LoggerSM_write(obj, evt, mid, a1, a2, 0, 0, 0, 0, 0, 0);
    }
    else {
        /* Figure out big the write will be */
        numBits8ToWrite = BITS8_IN_EVENTHDR + BITS8_IN_NUM_ARGS_2 +
                                                            BITS8_IN_TIMESTAMP;

        key = Gate_enterSystem();
        if (LoggerSM_getFreeSize() <= BITS8_TO_MAU(numBits8ToWrite)) {
            if (LoggerSM_overwrite == FALSE) {
                LoggerSM_module->serial++;
                LoggerSM_module->sharedObj->droppedEvents++;
                Gate_leaveSystem(key);
                return;
            }
            else {
                eraseRecords(BITS8_TO_MAU(numBits8ToWrite));
            }
        }
        myWrPtr = (Bits32 *)fillInEventHdr((UInt32 *)LoggerSM_module->sharedObj->writePtr,
                      numBits8ToWrite, LoggerSM_module->serial++);
        *(myWrPtr++) = ((evt) & 0xffff0000) | mid;
        *(myWrPtr++) = a1;
        *(myWrPtr++) = a2;
        LoggerSM_updateWritePtrByValue(myWrPtr);
        Gate_leaveSystem(key);
    }
}

/*
 *  ======== write4 =========
 *  Log an event with 4 parameters and the calling address
 */
Void LoggerSM_write4(LoggerSM_Object *obj, Log_Event evt, Types_ModuleId mid,
                     IArg a1, IArg a2, IArg a3, IArg a4)
{
    IArg key;
    Bits32 *myWrPtr;
    SizeT numBits8ToWrite;

    /* Do not do any work if there is no shared object yet */
    if (LoggerSM_module->sharedObj == NULL) {
        return;
    }

    if (LoggerSM_decode == TRUE) {
        LoggerSM_write(obj, evt, mid, a1, a2, a3, a4, 0, 0, 0, 0);
    }
    else {
        /* Figure out big the write will be */
        numBits8ToWrite = BITS8_IN_EVENTHDR + BITS8_IN_NUM_ARGS_4 +
                                                            BITS8_IN_TIMESTAMP;

        key = Gate_enterSystem();
        if (LoggerSM_getFreeSize() <= BITS8_TO_MAU(numBits8ToWrite)) {
            if (LoggerSM_overwrite == FALSE) {
                LoggerSM_module->serial++;
                LoggerSM_module->sharedObj->droppedEvents++;
                Gate_leaveSystem(key);
                return;
            }
            else {
                eraseRecords(BITS8_TO_MAU(numBits8ToWrite));
            }
        }
        myWrPtr = (Bits32 *)fillInEventHdr((UInt32 *)LoggerSM_module->sharedObj->writePtr,
                      numBits8ToWrite, LoggerSM_module->serial++);
        *(myWrPtr++) = ((evt) & 0xffff0000) | mid;
        *(myWrPtr++) = a1;
        *(myWrPtr++) = a2;
        *(myWrPtr++) = a3;
        *(myWrPtr++) = a4;
        LoggerSM_updateWritePtrByValue(myWrPtr);
        Gate_leaveSystem(key);
    }
}

/*
 *  ======== write8 =========
 *  Log an event with 8 parameters and the calling address
 */
Void LoggerSM_write8(LoggerSM_Object *obj, Log_Event evt, Types_ModuleId mid,
                     IArg a1, IArg a2, IArg a3, IArg a4, IArg a5, IArg a6,
                     IArg a7, IArg a8)
{
    IArg key;
    Bits32 *myWrPtr;
    SizeT numBits8ToWrite;

    /* Do not do any work if there is no shared object yet */
    if (LoggerSM_module->sharedObj == NULL) {
        return;
    }

    if (LoggerSM_decode == TRUE) {
        LoggerSM_write(obj, evt, mid, a1, a2, a3, a4, a5, a6, a7, a8);
    }
    else {
        /* Figure out big the write will be */
        numBits8ToWrite = BITS8_IN_EVENTHDR + BITS8_IN_NUM_ARGS_8 +
                                                            BITS8_IN_TIMESTAMP;

        key = Gate_enterSystem();
        if (LoggerSM_getFreeSize() <= BITS8_TO_MAU(numBits8ToWrite)) {
            if (LoggerSM_overwrite == FALSE) {
                LoggerSM_module->serial++;
                LoggerSM_module->sharedObj->droppedEvents++;
                Gate_leaveSystem(key);
                return;
            }
            else {
                eraseRecords(BITS8_TO_MAU(numBits8ToWrite));
            }
        }
        myWrPtr = (Bits32 *)fillInEventHdr((UInt32 *)LoggerSM_module->sharedObj->writePtr,
                      numBits8ToWrite, LoggerSM_module->serial++);
        *(myWrPtr++) = ((evt) & 0xffff0000) | mid;
        *(myWrPtr++) = a1;
        *(myWrPtr++) = a2;
        *(myWrPtr++) = a3;
        *(myWrPtr++) = a4;
        *(myWrPtr++) = a5;
        *(myWrPtr++) = a6;
        *(myWrPtr++) = a7;
        *(myWrPtr++) = a8;
        LoggerSM_updateWritePtrByValue(myWrPtr);
        Gate_leaveSystem(key);
    }
}
