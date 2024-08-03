/*
 * PROJECT:         ReactOS Boot Loader
 * LICENSE:         BSD - See /docs/licenses/COPYING.ARM
 * FILE:            boot/armllb/inc/keyboard.h
 * PURPOSE:         LLB Keyboard Functions
 * PROGRAMMERS:     ReactOS Portable Systems Group
 */

CHAR
NTAPI
LlbKeyboardGetChar(
    VOID
);

CHAR
NTAPI
LlbKeypadGetChar(
    VOID
);

/* EOF */
