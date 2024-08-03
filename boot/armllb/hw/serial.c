/*
 * PROJECT:         ReactOS Boot Loader
 * LICENSE:         BSD - See /docs/licenses/COPYING.ARM
 * FILE:            boot/armllb/hw/serial.c
 * PURPOSE:         LLB Serial Port Routines
 * PROGRAMMERS:     ReactOS Portable Systems Group
 */

#include "precomp.h"

VOID
NTAPI
LlbSerialPutChar(IN CHAR c)
{
    /* Properly support new-lines */
    if (c == '\n') LlbSerialPutChar('\r');

    /* Wait for ready */
    while (!LlbHwUartTxReady());

    /* Send character */
    LlbHwUartSendChar(c);
}

/* EOF */
