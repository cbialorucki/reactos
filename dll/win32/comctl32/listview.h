/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Listview control header file
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#pragma once

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "windef.h"
#include "winbase.h"
#include "winnt.h"
#include "wingdi.h"
#include "winuser.h"
#include "winnls.h"
#include "commctrl.h"
#include "comctl32.h"
#include "uxtheme.h"

#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(listview);

typedef struct tagCOLUMN_INFO
{
  RECT rcHeader;	/* tracks the header's rectangle */
  INT fmt;		/* same as LVCOLUMN.fmt */
  INT cxMin;
} COLUMN_INFO;

typedef struct tagITEMHDR
{
  LPWSTR pszText;
  INT iImage;
} ITEMHDR, *LPITEMHDR;

typedef struct tagSUBITEM_INFO
{
  ITEMHDR hdr;
  INT iSubItem;
} SUBITEM_INFO;

typedef struct tagITEM_ID ITEM_ID;

typedef struct tagITEM_INFO
{
  ITEMHDR hdr;
  UINT state;
  LPARAM lParam;
  INT iIndent;
  ITEM_ID *id;
} ITEM_INFO;

struct tagITEM_ID
{
  UINT id;   /* item id */
  HDPA item; /* link to item data */
};

typedef struct tagRANGE
{
  INT lower;
  INT upper;
} RANGE;

typedef struct tagRANGES
{
  HDPA hdpa;
} *RANGES;

typedef struct tagITERATOR
{
  INT nItem;
  INT nSpecial;
  RANGE range;
  RANGES ranges;
  INT index;
} ITERATOR;

typedef struct tagDELAYED_ITEM_EDIT
{
  BOOL fEnabled;
  INT iItem;
} DELAYED_ITEM_EDIT;

enum notification_mask
{
  NOTIFY_MASK_ITEM_CHANGE = 0x1,
  NOTIFY_MASK_END_LABEL_EDIT = 0x2,
  NOTIFY_MASK_UNMASK_ALL = 0xffffffff
};

typedef struct tagLISTVIEW_INFO
{
  /* control window */
  HWND hwndSelf;
  RECT rcList;                 /* This rectangle is really the window
				* client rectangle possibly reduced by the
				* horizontal scroll bar and/or header - see
				* LISTVIEW_UpdateSize. This rectangle offset
				* by the LISTVIEW_GetOrigin value is in
				* client coordinates   */

  /* notification window */
  SHORT notifyFormat;
  HWND hwndNotify;
  DWORD notify_mask;
  UINT uCallbackMask;

  /* tooltips */
  HWND hwndToolTip;

  /* items */
  INT nItemCount;		/* the number of items in the list */
  HDPA hdpaItems;               /* array ITEM_INFO pointers */
  HDPA hdpaItemIds;             /* array of ITEM_ID pointers */
  HDPA hdpaPosX;		/* maintains the (X, Y) coordinates of the */
  HDPA hdpaPosY;		/* items in LVS_ICON, and LVS_SMALLICON modes */
  RANGES selectionRanges;
  INT nSelectionMark;           /* item to start next multiselection from */
  INT nHotItem;

  /* columns */
  HDPA hdpaColumns;		/* array of COLUMN_INFO pointers */
  BOOL colRectsDirty;		/* trigger column rectangles requery from header */

  /* item metrics */
  BOOL bNoItemMetrics;		/* flags if item metrics are not yet computed */
  INT nItemHeight;
  INT nItemWidth;

  /* sorting */
  PFNLVCOMPARE pfnCompare;      /* sorting callback pointer */
  LPARAM lParamSort;

  /* style */
  DWORD dwStyle;		/* the cached window GWL_STYLE */
  DWORD dwLvExStyle;		/* extended listview style */
  DWORD uView;			/* current view available through LVM_[G,S]ETVIEW */

  /* edit item */
  HWND hwndEdit;
  WNDPROC EditWndProc;
  INT nEditLabelItem;
  DELAYED_ITEM_EDIT itemEdit;   /* Pointer to this structure will be the timer ID */

  /* icons */
  HIMAGELIST himlNormal;
  HIMAGELIST himlSmall;
  HIMAGELIST himlState;
  SIZE iconSize;
  BOOL autoSpacing;
  SIZE iconSpacing;
  SIZE iconStateSize;
  POINT currIconPos;        /* this is the position next icon will be placed */

  /* header */
  HWND hwndHeader;
  INT xTrackLine;           /* The x coefficient of the track line or -1 if none */

  /* marquee selection */
  BOOL bMarqueeSelect;      /* marquee selection/highlight underway */
  BOOL bScrolling;
  RECT marqueeRect;         /* absolute coordinates of marquee selection */
  RECT marqueeDrawRect;     /* relative coordinates for drawing marquee */
  POINT marqueeOrigin;      /* absolute coordinates of marquee click origin */

  /* focus drawing */
  BOOL bFocus;              /* control has focus */
  INT nFocusedItem;
  RECT rcFocus;             /* focus bounds */

  /* colors */
  HBRUSH hBkBrush;
  COLORREF clrBk;
  COLORREF clrText;
  COLORREF clrTextBk;
  BOOL bDefaultBkColor;

  /* font */
  HFONT hDefaultFont;
  HFONT hFont;
  INT ntmHeight;            /* Some cached metrics of the font used */
  INT ntmMaxCharWidth;      /* by the listview to draw items */
  INT nEllipsisWidth;

  /* mouse operation */
  BOOL bLButtonDown;
  BOOL bDragging;
  POINT ptClickPos;         /* point where the user clicked */
  INT nLButtonDownItem;     /* tracks item to reset multiselection on WM_LBUTTONUP */
  DWORD dwHoverTime;
  HCURSOR hHotCursor;
  INT cWheelRemainder;

  /* keyboard operation */
  DWORD lastKeyPressTimestamp;
  WPARAM charCode;
  INT nSearchParamLength;
  WCHAR szSearchParam[ MAX_PATH ];

  /* painting */
  BOOL bIsDrawing;         /* Drawing in progress */
  INT nMeasureItemHeight;  /* WM_MEASUREITEM result */
  BOOL redraw;             /* WM_SETREDRAW switch */

  /* misc */
  DWORD iVersion;          /* CCM_[G,S]ETVERSION */
} LISTVIEW_INFO;

/*
 * constants
 */
/* How many we debug buffer to allocate */
#define DEBUG_BUFFERS 20
/* The size of a single debug buffer */
#define DEBUG_BUFFER_SIZE 256

/* Internal interface to LISTVIEW_HScroll and LISTVIEW_VScroll */
#define SB_INTERNAL      -1

/* maximum size of a label */
#define DISP_TEXT_SIZE 260

/* padding for items in list and small icon display modes */
#define WIDTH_PADDING 12

/* padding for items in list, report and small icon display modes */
#define HEIGHT_PADDING 1

/* offset of items in report display mode */
#define REPORT_MARGINX 2

/* padding for icon in large icon display mode
 *   ICON_TOP_PADDING_NOTHITABLE - space between top of box and area
 *                                 that HITTEST will see.
 *   ICON_TOP_PADDING_HITABLE - spacing between above and icon.
 *   ICON_TOP_PADDING - sum of the two above.
 *   ICON_BOTTOM_PADDING - between bottom of icon and top of text
 *   LABEL_HOR_PADDING - between text and sides of box
 *   LABEL_VERT_PADDING - between bottom of text and end of box
 *
 *   ICON_LR_PADDING - additional width above icon size.
 *   ICON_LR_HALF - half of the above value
 */
#define ICON_TOP_PADDING_NOTHITABLE  2
#define ICON_TOP_PADDING_HITABLE     2
#define ICON_TOP_PADDING (ICON_TOP_PADDING_NOTHITABLE + ICON_TOP_PADDING_HITABLE)
#define ICON_BOTTOM_PADDING          4
#define LABEL_HOR_PADDING            5
#define LABEL_VERT_PADDING           7
#define ICON_LR_PADDING              16
#define ICON_LR_HALF                 (ICON_LR_PADDING/2)

/* default label width for items in list and small icon display modes */
#define DEFAULT_LABEL_WIDTH 40
/* maximum select rectangle width for empty text item in LV_VIEW_DETAILS */
#define MAX_EMPTYTEXT_SELECT_WIDTH 80

/* default column width for items in list display mode */
#define DEFAULT_COLUMN_WIDTH 128

/* Size of "line" scroll for V & H scrolls */
#define LISTVIEW_SCROLL_ICON_LINE_SIZE 37

/* Padding between image and label */
#define IMAGE_PADDING  2

/* Padding behind the label */
#define TRAILING_LABEL_PADDING  12
#define TRAILING_HEADER_PADDING  11

/* Border for the icon caption */
#define CAPTION_BORDER  2

/* Standard DrawText flags */
#define LV_ML_DT_FLAGS  (DT_TOP | DT_NOPREFIX | DT_EDITCONTROL | DT_CENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS | DT_END_ELLIPSIS)
#define LV_FL_DT_FLAGS  (DT_TOP | DT_NOPREFIX | DT_EDITCONTROL | DT_CENTER | DT_WORDBREAK | DT_NOCLIP)
#define LV_SL_DT_FLAGS  (DT_VCENTER | DT_NOPREFIX | DT_EDITCONTROL | DT_SINGLELINE | DT_WORD_ELLIPSIS | DT_END_ELLIPSIS)

/* Image index from state */
#define STATEIMAGEINDEX(x) (((x) & LVIS_STATEIMAGEMASK) >> 12)

/* The time in milliseconds to reset the search in the list */
#define KEY_DELAY       450

/* Dump the LISTVIEW_INFO structure to the debug channel */
#define LISTVIEW_DUMP(iP) do { \
  TRACE("hwndSelf=%p, clrBk=0x%06x, clrText=0x%06x, clrTextBk=0x%06x, ItemHeight=%d, ItemWidth=%d, Style=0x%08x\n", \
        iP->hwndSelf, iP->clrBk, iP->clrText, iP->clrTextBk, \
        iP->nItemHeight, iP->nItemWidth, iP->dwStyle); \
  TRACE("hwndSelf=%p, himlNor=%p, himlSml=%p, himlState=%p, Focused=%d, Hot=%d, exStyle=0x%08x, Focus=%d\n", \
        iP->hwndSelf, iP->himlNormal, iP->himlSmall, iP->himlState, \
        iP->nFocusedItem, iP->nHotItem, iP->dwLvExStyle, iP->bFocus ); \
  TRACE("hwndSelf=%p, ntmH=%d, icSz.cx=%d, icSz.cy=%d, icSp.cx=%d, icSp.cy=%d, notifyFmt=%d\n", \
        iP->hwndSelf, iP->ntmHeight, iP->iconSize.cx, iP->iconSize.cy, \
        iP->iconSpacing.cx, iP->iconSpacing.cy, iP->notifyFormat); \
  TRACE("hwndSelf=%p, rcList=%s\n", iP->hwndSelf, wine_dbgstr_rect(&iP->rcList)); \
} while(0)

static const WCHAR themeClass[] = L"ListView";

static BOOL LISTVIEW_GetItemT(const LISTVIEW_INFO *, LPLVITEMW, BOOL);
static void LISTVIEW_GetItemBox(const LISTVIEW_INFO *, INT, LPRECT);
static void LISTVIEW_GetItemOrigin(const LISTVIEW_INFO *, INT, LPPOINT);
static BOOL LISTVIEW_GetItemPosition(const LISTVIEW_INFO *, INT, LPPOINT);
static BOOL LISTVIEW_GetItemRect(const LISTVIEW_INFO *, INT, LPRECT);
static void LISTVIEW_GetOrigin(const LISTVIEW_INFO *, LPPOINT);
static BOOL LISTVIEW_GetViewRect(const LISTVIEW_INFO *, LPRECT);
static void LISTVIEW_UpdateSize(LISTVIEW_INFO *);
static LRESULT LISTVIEW_Command(LISTVIEW_INFO *, WPARAM, LPARAM);
static INT LISTVIEW_GetStringWidthT(const LISTVIEW_INFO *, LPCWSTR, BOOL);
static BOOL LISTVIEW_KeySelection(LISTVIEW_INFO *, INT, BOOL);
static UINT LISTVIEW_GetItemState(const LISTVIEW_INFO *, INT, UINT);
static BOOL LISTVIEW_SetItemState(LISTVIEW_INFO *, INT, const LVITEMW *);
static LRESULT LISTVIEW_VScroll(LISTVIEW_INFO *, INT, INT);
static LRESULT LISTVIEW_HScroll(LISTVIEW_INFO *, INT, INT);
static BOOL LISTVIEW_EnsureVisible(LISTVIEW_INFO *, INT, BOOL);
static HIMAGELIST LISTVIEW_SetImageList(LISTVIEW_INFO *, INT, HIMAGELIST);
static INT LISTVIEW_HitTest(const LISTVIEW_INFO *, LPLVHITTESTINFO, BOOL, BOOL);
static BOOL LISTVIEW_EndEditLabelT(LISTVIEW_INFO *, BOOL, BOOL);
static BOOL LISTVIEW_Scroll(LISTVIEW_INFO *, INT, INT);