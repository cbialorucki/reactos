/*
 * PROJECT:         ReactOS HAL
 * LICENSE:         BSD - See /docs/licenses/COPYING.ARM
 * FILE:            hal/halarm/generic/fmutex.c
 * PURPOSE:         Fast Mutex Support
 * PROGRAMMERS:     ReactOS Portable Systems Group
 */

/* INCLUDES *******************************************************************/

#include <hal.h>
#define NDEBUG
#include <debug.h>

#undef ExAcquireFastMutex
#undef ExReleaseFastMutex
#undef ExTryToAcquireFastMutex

/* FUNCTIONS ******************************************************************/

VOID
FASTCALL
ExAcquireFastMutex(IN PFAST_MUTEX FastMutex)
{
    UNIMPLEMENTED;
    while (TRUE);
}

VOID
FASTCALL
ExReleaseFastMutex(IN PFAST_MUTEX FastMutex)
{
    UNIMPLEMENTED;
    while (TRUE);
}

BOOLEAN
FASTCALL
ExTryToAcquireFastMutex(IN PFAST_MUTEX FastMutex)
{
    UNIMPLEMENTED;
    while (TRUE);
    return TRUE;
}

/* EOF */
