/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Tooltip control header file
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#pragma once

#include <stdarg.h>
#include <string.h>

#include "windef.h"
#include "winbase.h"
#include "wine/unicode.h"
#include "wingdi.h"
#include "winuser.h"
#include "winnls.h"
#include "commctrl.h"
#include "comctl32.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(tooltips);

static HICON hTooltipIcons[TTI_ERROR+1];

typedef struct
{
    UINT      uFlags;
    UINT      uInternalFlags;
    HWND      hwnd;
    BOOL      bNotifyUnicode;
    UINT_PTR  uId;
    RECT      rect;
    HINSTANCE hinst;
    LPWSTR      lpszText;
    LPARAM      lParam;
} TTTOOL_INFO;


typedef struct
{
    HWND     hwndSelf;
    WCHAR      szTipText[INFOTIPSIZE];
    BOOL     bActive;
    BOOL     bTrackActive;
    UINT     uNumTools;
    COLORREF   clrBk;
    COLORREF   clrText;
    HFONT    hFont;
    HFONT    hTitleFont;
    INT      xTrackPos;
    INT      yTrackPos;
    INT      nMaxTipWidth;
    INT      nTool; /* tool that mouse was on on last relayed mouse move */
    INT      nCurrentTool;
    INT      nTrackTool;
    INT      nReshowTime;
    INT      nAutoPopTime;
    INT      nInitialTime;
    RECT     rcMargin;
    BOOL     bToolBelow;
    LPWSTR   pszTitle;
    HICON    hTitleIcon;
    TTTOOL_INFO *tools;
} TOOLTIPS_INFO;

#define ID_TIMERSHOW   1    /* show delay timer */
#define ID_TIMERPOP    2    /* auto pop timer */
#define ID_TIMERLEAVE  3    /* tool leave timer */


#define TOOLTIPS_GetInfoPtr(hWindow) ((TOOLTIPS_INFO *)GetWindowLongPtrW (hWindow, 0))

/* offsets from window edge to start of text */
#define NORMAL_TEXT_MARGIN 2
#define BALLOON_TEXT_MARGIN (NORMAL_TEXT_MARGIN+8)
/* value used for CreateRoundRectRgn that specifies how much
 * each corner is curved */
#define BALLOON_ROUNDEDNESS 16
#define BALLOON_STEMHEIGHT 18
#define BALLOON_STEMWIDTH 18
#define BALLOON_STEMINDENT 16

#define BALLOON_ICON_TITLE_SPACING 8 /* horizontal spacing between icon and title */
#define BALLOON_TITLE_TEXT_SPACING 8 /* vertical spacing between icon/title and main text */
#define ICON_HEIGHT 16
#define ICON_WIDTH  16

#define MAX_TEXT_SIZE_A 80 /* maximum retrieving text size by ANSI message */