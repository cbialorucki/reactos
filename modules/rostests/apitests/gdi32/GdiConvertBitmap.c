/*
 * PROJECT:         ReactOS api tests
 * LICENSE:         GPL - See /docs/licenses/COPYING
 * PURPOSE:         Test for GdiConvertBitmap
 * PROGRAMMERS:     Timo Kreuzer
 */

#include "precomp.h"

HBITMAP WINAPI GdiConvertBitmap(HBITMAP hbm);

void Test_GdiConvertBitmap()
{
    ok(GdiConvertBitmap((HBITMAP)-1) == (HBITMAP)-1, "\n");
    ok(GdiConvertBitmap((HBITMAP)0) == (HBITMAP)0, "\n");
    ok(GdiConvertBitmap((HBITMAP)1) == (HBITMAP)1, "\n");
    ok(GdiConvertBitmap((HBITMAP)2) == (HBITMAP)2, "\n");
}

START_TEST(GdiConvertBitmap)
{
    Test_GdiConvertBitmap();
}

