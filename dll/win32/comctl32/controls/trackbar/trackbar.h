/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Trackbar control header file
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "winnls.h"
#include "commctrl.h"
#include "uxtheme.h"
#include "vssym32.h"
#include "wine/debug.h"

#include "comctl32.h"

WINE_DEFAULT_DEBUG_CHANNEL(trackbar);

typedef struct
{
    HWND hwndSelf;
    DWORD dwStyle;
    LONG lRangeMin;
    LONG lRangeMax;
    LONG lLineSize;
    LONG lPageSize;
    LONG lSelMin;
    LONG lSelMax;
    LONG lPos;
    UINT uThumbLen;
    UINT uNumTics;
    UINT uTicFreq;
    HWND hwndNotify;
    HWND hwndToolTip;
    HWND hwndBuddyLA;
    HWND hwndBuddyRB;
    INT  fLocation;
    INT  flags;
    BOOL bUnicode;
    BOOL bFocussed;
    RECT rcChannel;
    RECT rcSelection;
    RECT rcThumb;
    LPLONG tics;
} TRACKBAR_INFO;

#define TB_REFRESH_TIMER	1
#define TB_REFRESH_DELAY	500

#define TOOLTIP_OFFSET		2     /* distance from ctrl edge to tooltip */

#define TB_DEFAULTPAGESIZE	20

/* Used by TRACKBAR_Refresh to find out which parts of the control
   need to be recalculated */

#define TB_THUMBPOSCHANGED      1
#define TB_THUMBSIZECHANGED     2
#define TB_THUMBCHANGED 	(TB_THUMBPOSCHANGED | TB_THUMBSIZECHANGED)
#define TB_SELECTIONCHANGED     4
#define TB_DRAG_MODE            8     /* we're dragging the slider */
#define TB_AUTO_PAGE_LEFT	16
#define TB_AUTO_PAGE_RIGHT	32
#define TB_AUTO_PAGE		(TB_AUTO_PAGE_LEFT | TB_AUTO_PAGE_RIGHT)
#define TB_THUMB_HOT            64    /* mouse hovers above thumb */

/* helper defines for TRACKBAR_DrawTic */
#define TIC_EDGE                0x20
#define TIC_SELECTIONMARKMAX    0x80
#define TIC_SELECTIONMARKMIN    0x100
#define TIC_SELECTIONMARK       (TIC_SELECTIONMARKMAX | TIC_SELECTIONMARKMIN)

static const WCHAR themeClass[] = L"Trackbar";