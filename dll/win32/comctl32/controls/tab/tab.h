/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Tab control header file
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#pragma once

#include <assert.h>
#include <stdarg.h>
#include <string.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "winnls.h"
#include "commctrl.h"
#include "comctl32.h"
#include "uxtheme.h"
#include "vssym32.h"
#include "wine/debug.h"
#include <math.h>

WINE_DEFAULT_DEBUG_CHANNEL(tab);

typedef struct
{
  DWORD  dwState;
  LPWSTR pszText;
  INT    iImage;
  RECT   rect;      /* bounding rectangle of the item relative to the
                     * leftmost item (the leftmost item, 0, would have a
                     * "left" member of 0 in this rectangle)
                     *
                     * additionally the top member holds the row number
                     * and bottom is unused and should be 0 */
  BYTE   extra[1];  /* Space for caller supplied info, variable size */
} TAB_ITEM;

/* The size of a tab item depends on how much extra data is requested.
   TCM_INSERTITEM always stores at least LPARAM sized data. */
#define EXTRA_ITEM_SIZE(infoPtr) (max((infoPtr)->cbInfo, (INT)sizeof(LPARAM)))
#define TAB_ITEM_SIZE(infoPtr) FIELD_OFFSET(TAB_ITEM, extra[EXTRA_ITEM_SIZE(infoPtr)])

typedef struct
{
  HWND       hwnd;            /* Tab control window */
  HWND       hwndNotify;      /* notification window (parent) */
  UINT       uNumItem;        /* number of tab items */
  UINT       uNumRows;	      /* number of tab rows */
  INT        tabHeight;       /* height of the tab row */
  INT        tabWidth;        /* width of tabs */
  INT        tabMinWidth;     /* minimum width of items */
  USHORT     uHItemPadding;   /* amount of horizontal padding, in pixels */
  USHORT     uVItemPadding;   /* amount of vertical padding, in pixels */
  USHORT     uHItemPadding_s; /* Set amount of horizontal padding, in pixels */
  USHORT     uVItemPadding_s; /* Set amount of vertical padding, in pixels */
  HFONT      hFont;           /* handle to the current font */
  HCURSOR    hcurArrow;       /* handle to the current cursor */
  HIMAGELIST himl;            /* handle to an image list (may be 0) */
  HWND       hwndToolTip;     /* handle to tab's tooltip */
  INT        leftmostVisible; /* Used for scrolling, this member contains
                               * the index of the first visible item */
  INT        iSelected;       /* the currently selected item */
  INT        iHotTracked;     /* the highlighted item under the mouse */
  INT        uFocus;          /* item which has the focus */
  BOOL       DoRedraw;        /* flag for redrawing when tab contents is changed*/
  BOOL       needsScrolling;  /* TRUE if the size of the tabs is greater than
                               * the size of the control */
  BOOL       fHeightSet;      /* was the height of the tabs explicitly set? */
  BOOL       bUnicode;        /* Unicode control? */
  HWND       hwndUpDown;      /* Updown control used for scrolling */
  INT        cbInfo;          /* Number of bytes of caller supplied info per tab */

  DWORD      exStyle;         /* Extended style used, currently:
                                 TCS_EX_FLATSEPARATORS, TCS_EX_REGISTERDROP */
  DWORD      dwStyle;         /* the cached window GWL_STYLE */

  HDPA       items;           /* dynamic array of TAB_ITEM* pointers */
} TAB_INFO;

/******************************************************************************
 * Positioning constants
 */
#define SELECTED_TAB_OFFSET     2
#define ROUND_CORNER_SIZE       2
#define DISPLAY_AREA_PADDINGX   2
#define DISPLAY_AREA_PADDINGY   2
#define CONTROL_BORDER_SIZEX    2
#define CONTROL_BORDER_SIZEY    2
#define BUTTON_SPACINGX         3
#define BUTTON_SPACINGY         3
#define FLAT_BTN_SPACINGX       8
#define DEFAULT_MIN_TAB_WIDTH   54
#define DEFAULT_PADDING_X       6
#define EXTRA_ICON_PADDING      3

#define TAB_GetInfoPtr(hwnd) ((TAB_INFO *)GetWindowLongPtrW(hwnd,0))

#define GET_DEFAULT_MIN_TAB_WIDTH(infoPtr) (DEFAULT_MIN_TAB_WIDTH - (DEFAULT_PADDING_X - (infoPtr)->uHItemPadding) * 2)

/******************************************************************************
 * Hot-tracking timer constants
 */
#define TAB_HOTTRACK_TIMER            1
#define TAB_HOTTRACK_TIMER_INTERVAL   100   /* milliseconds */

static const WCHAR themeClass[] = L"Tab";