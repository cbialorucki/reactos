/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Header control header file
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#pragma once

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "windef.h"
#include "winbase.h"
#include "wine/unicode.h"
#include "wingdi.h"
#include "winuser.h"
#include "winnls.h"
#include "commctrl.h"
#include "comctl32.h"
#include "vssym32.h"
#include "uxtheme.h"
#include "wine/debug.h"
#include "wine/heap.h"

WINE_DEFAULT_DEBUG_CHANNEL(header);

typedef struct
{
    INT     cxy;
    HBITMAP hbm;
    LPWSTR    pszText;
    INT     fmt;
    LPARAM    lParam;
    INT     iImage;
    INT     iOrder;		/* see documentation of HD_ITEM */

    BOOL    bDown;		/* is item pressed? (used for drawing) */
    RECT    rect;		/* bounding rectangle of the item */
    DWORD   callbackMask;       /* HDI_* flags for items that are callback */
} HEADER_ITEM;


typedef struct
{
    HWND      hwndSelf;		/* Control window */
    HWND      hwndNotify;	/* Owner window to send notifications to */
    INT       nNotifyFormat;	/* format used for WM_NOTIFY messages */
    UINT      uNumItem;		/* number of items (columns) */
    INT       nHeight;		/* height of the header (pixels) */
    HFONT     hFont;		/* handle to the current font */
    HCURSOR   hcurArrow;	/* handle to the arrow cursor */
    HCURSOR   hcurDivider;	/* handle to a cursor (used over dividers) <-|-> */
    HCURSOR   hcurDivopen;	/* handle to a cursor (used over dividers) <-||-> */
    BOOL      bCaptured;	/* Is the mouse captured? */
    BOOL      bPressed;		/* Is a header item pressed (down)? */
    BOOL      bDragging;        /* Are we dragging an item? */
    BOOL      bTracking;	/* Is in tracking mode? */
    POINT     ptLButtonDown;    /* The point where the left button was pressed */
    DWORD     dwStyle;		/* the cached window GWL_STYLE */
    INT       iMoveItem;	/* index of tracked item. (Tracking mode) */
    INT       xTrackOffset;	/* distance between the right side of the tracked item and the cursor */
    INT       xOldTrack;	/* track offset (see above) after the last WM_MOUSEMOVE */
    INT       iHotItem;		/* index of hot item (cursor is over this item) */
    INT       iHotDivider;      /* index of the hot divider (used while dragging an item or by HDM_SETHOTDIVIDER) */
    INT       iMargin;          /* width of the margin that surrounds a bitmap */
    INT       filter_change_timeout; /* change timeout set with HDM_SETFILTERCHANGETIMEOUT */

    HIMAGELIST  himl;		/* handle to an image list (may be 0) */
    HEADER_ITEM *items;		/* pointer to array of HEADER_ITEM's */
    INT         *order;         /* array of item IDs indexed by order */
    BOOL	bRectsValid;	/* validity flag for bounding rectangles */
} HEADER_INFO;


#define VERT_BORDER     4
#define DIVIDER_WIDTH  10
#define HOT_DIVIDER_WIDTH 2
#define MAX_HEADER_TEXT_LEN 260
#define HDN_UNICODE_OFFSET 20
#define HDN_FIRST_UNICODE (HDN_FIRST-HDN_UNICODE_OFFSET)

#define HDI_SUPPORTED_FIELDS (HDI_WIDTH|HDI_TEXT|HDI_FORMAT|HDI_LPARAM|HDI_BITMAP|HDI_IMAGE|HDI_ORDER)
#define HDI_UNSUPPORTED_FIELDS (HDI_FILTER)
#define HDI_UNKNOWN_FIELDS (~(HDI_SUPPORTED_FIELDS|HDI_UNSUPPORTED_FIELDS|HDI_DI_SETITEM))
#define HDI_COMCTL32_4_0_FIELDS (HDI_WIDTH|HDI_TEXT|HDI_FORMAT|HDI_LPARAM|HDI_BITMAP)


static BOOL HEADER_PrepareCallbackItems(const HEADER_INFO *infoPtr, INT iItem, INT reqMask);
static void HEADER_FreeCallbackItems(HEADER_ITEM *lpItem);
static LRESULT HEADER_SendNotify(const HEADER_INFO *infoPtr, UINT code, NMHDR *hdr);
static LRESULT HEADER_SendCtrlCustomDraw(const HEADER_INFO *infoPtr, DWORD dwDrawStage, HDC hdc, const RECT *rect);

static const WCHAR themeClass[] = L"Header";