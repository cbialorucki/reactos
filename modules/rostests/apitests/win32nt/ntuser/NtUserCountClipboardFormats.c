/*
 * PROJECT:         ReactOS api tests
 * LICENSE:         GPL - See /docs/licenses/COPYING
 * PURPOSE:         Test for NtUserCountClipboardFormats
 * PROGRAMMERS:
 */

#include "../win32nt.h"


START_TEST(NtUserCountClipboardFormats)
{
    RTEST(NtUserCountClipboardFormats() < 1000);
}

