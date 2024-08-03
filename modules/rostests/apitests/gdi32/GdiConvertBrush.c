/*
 * PROJECT:         ReactOS api tests
 * LICENSE:         GPL - See /docs/licenses/COPYING
 * PURPOSE:         Test for GdiConvertBrush
 * PROGRAMMERS:     Timo Kreuzer
 */

#include "precomp.h"

HBRUSH WINAPI GdiConvertBrush(HBRUSH hbr);

void Test_GdiConvertBrush()
{
    ok(GdiConvertBrush((HBRUSH)-1) == (HBRUSH)-1, "\n");
    ok(GdiConvertBrush((HBRUSH)0) == (HBRUSH)0, "\n");
    ok(GdiConvertBrush((HBRUSH)1) == (HBRUSH)1, "\n");
    ok(GdiConvertBrush((HBRUSH)2) == (HBRUSH)2, "\n");
}

START_TEST(GdiConvertBrush)
{
    Test_GdiConvertBrush();
}

