/*
 * PROJECT:         ReactOS Boot Loader
 * LICENSE:         BSD - See /docs/licenses/COPYING.ARM
 * FILE:            boot/armllb/inc/versa.h
 * PURPOSE:         LLB Board-Specific Hardware Functions for Versatile
 * PROGRAMMERS:     ReactOS Portable Systems Group
 */

VOID
NTAPI
LlbHwVersaUartInitialize(
    VOID
);

VOID
NTAPI
LlbHwVersaClcdInitialize(
    VOID
);

VOID
NTAPI
LlbHwVersaKmiInitialize(
    VOID
);

/* EOF */
