/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Treeview control header file
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#pragma once

#include "config.h"
//#include "wine/port.h"

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#define NONAMELESSUNION

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "winnls.h"
#include "commctrl.h"
#include "comctl32.h"
#include "uxtheme.h"
#include "vssym32.h"
#include "wine/unicode.h"
#include "wine/debug.h"
#include "wine/exception.h"
#include "wine/heap.h"

WINE_DEFAULT_DEBUG_CHANNEL(treeview);

/* internal structures */
typedef struct tagTREEVIEW_INFO
{
  HWND          hwnd;
  HWND          hwndNotify;     /* Owner window to send notifications to */
  DWORD         dwStyle;
  HTREEITEM     root;
  UINT          uInternalStatus;
  INT           Timer;
  UINT          uNumItems;      /* number of valid TREEVIEW_ITEMs */
  INT           cdmode;         /* last custom draw setting */
  UINT          uScrollTime;	/* max. time for scrolling in milliseconds */
  BOOL          bRedraw;        /* if FALSE we validate but don't redraw in TREEVIEW_Paint() */

  UINT          uItemHeight;    /* item height */
  BOOL          bHeightSet;

  LONG          clientWidth;    /* width of control window */
  LONG          clientHeight;   /* height of control window */

  LONG          treeWidth;      /* width of visible tree items */
  LONG          treeHeight;     /* height of visible tree items */

  UINT          uIndent;        /* indentation in pixels */
  HTREEITEM     selectedItem;   /* handle to selected item or 0 if none */
  HTREEITEM     hotItem;        /* handle currently under cursor, 0 if none */
  HTREEITEM	focusedItem;    /* item that was under the cursor when WM_LBUTTONDOWN was received */
  HTREEITEM     editItem;       /* item being edited with builtin edit box */

  HTREEITEM     firstVisible;   /* handle to item whose top edge is at y = 0 */
  LONG          maxVisibleOrder;
  HTREEITEM     dropItem;       /* handle to item selected by drag cursor */
  HTREEITEM     insertMarkItem; /* item after which insertion mark is placed */
  BOOL          insertBeforeorAfter; /* flag used by TVM_SETINSERTMARK */
  HIMAGELIST    dragList;       /* Bitmap of dragged item */
  LONG          scrollX;
  INT           wheelRemainder;
  COLORREF      clrBk;
  COLORREF      clrText;
  COLORREF      clrLine;
  COLORREF      clrInsertMark;
  HFONT         hFont;
  HFONT         hDefaultFont;
  HFONT         hBoldFont;
  HFONT         hUnderlineFont;
  HFONT         hBoldUnderlineFont;
  HCURSOR       hcurHand;
  HWND          hwndToolTip;

  HWND          hwndEdit;
  WNDPROC       wpEditOrig;     /* orig window proc for subclassing edit */
  BOOL          bIgnoreEditKillFocus;
  BOOL          bLabelChanged;

  BOOL          bNtfUnicode;    /* TRUE if should send NOTIFY with W */
  HIMAGELIST    himlNormal;
  int           normalImageHeight;
  int           normalImageWidth;
  HIMAGELIST    himlState;
  int           stateImageHeight;
  int           stateImageWidth;
  HDPA          items;

  DWORD lastKeyPressTimestamp;
  WPARAM charCode;
  INT nSearchParamLength;
  WCHAR szSearchParam[ MAX_PATH ];
} TREEVIEW_INFO;

typedef struct _TREEITEM    /* HTREEITEM is a _TREEINFO *. */
{
  HTREEITEM parent;         /* handle to parent or 0 if at root */
  HTREEITEM nextSibling;    /* handle to next item in list, 0 if last */
  HTREEITEM firstChild;     /* handle to first child or 0 if no child */

  UINT      callbackMask;
  UINT      state;
  UINT      stateMask;
  LPWSTR    pszText;
  int       cchTextMax;
  int       iImage;
  int       iSelectedImage;
  int       iExpandedImage;
  int       cChildren;
  LPARAM    lParam;
  int       iIntegral;      /* item height multiplier (1 is normal) */
  int       iLevel;         /* indentation level:0=root level */
  HTREEITEM lastChild;
  HTREEITEM prevSibling;    /* handle to prev item in list, 0 if first */
  RECT      rect;
  LONG      linesOffset;
  LONG      stateOffset;
  LONG      imageOffset;
  LONG      textOffset;
  LONG      textWidth;      /* horizontal text extent for pszText */
  LONG      visibleOrder;   /* Depth-first numbering of the items whose ancestors are all expanded,
                               corresponding to a top-to-bottom ordering in the tree view.
                               Each item takes up "item.iIntegral" spots in the visible order.
                               0 is the root's first child. */
  const TREEVIEW_INFO *infoPtr; /* tree data this item belongs to */
} TREEVIEW_ITEM;

/******** Defines that TREEVIEW_ProcessLetterKeys uses ****************/
#define KEY_DELAY       450

/* bitflags for infoPtr->uInternalStatus */

#define TV_HSCROLL 	0x01    /* treeview too large to fit in window */
#define TV_VSCROLL 	0x02	/* (horizontal/vertical) */
#define TV_LDRAG		0x04	/* Lbutton pushed to start drag */
#define TV_LDRAGGING	0x08	/* Lbutton pushed, mouse moved. */
#define TV_RDRAG		0x10	/* ditto Rbutton */
#define TV_RDRAGGING	0x20

/* bitflags for infoPtr->timer */

#define TV_EDIT_TIMER    2
#define TV_EDIT_TIMER_SET 2

#define TEXT_CALLBACK_SIZE 260

#define TREEVIEW_LEFT_MARGIN 8

#define MINIMUM_INDENT 19

#define CALLBACK_MASK_ALL (TVIF_TEXT|TVIF_CHILDREN|TVIF_IMAGE|TVIF_SELECTEDIMAGE)

#define STATEIMAGEINDEX(x) (((x) >> 12) & 0x0f)
#define OVERLAYIMAGEINDEX(x) (((x) >> 8) & 0x0f)
#define ISVISIBLE(x)         ((x)->visibleOrder >= 0)

#define GETLINECOLOR(x) ((x) == CLR_DEFAULT ? comctl32_color.clrGrayText   : (x))
#define GETBKCOLOR(x)   ((x) == CLR_NONE    ? comctl32_color.clrWindow     : (x))
#define GETTXTCOLOR(x)  ((x) == CLR_NONE    ? comctl32_color.clrWindowText : (x))
#define GETINSCOLOR(x)  ((x) == CLR_DEFAULT ? comctl32_color.clrBtnText    : (x))

static const WCHAR themeClass[] = L"Treeview";


typedef VOID (*TREEVIEW_ItemEnumFunc)(TREEVIEW_INFO *, TREEVIEW_ITEM *,LPVOID);


static VOID TREEVIEW_Invalidate(const TREEVIEW_INFO *, const TREEVIEW_ITEM *);

static LRESULT TREEVIEW_DoSelectItem(TREEVIEW_INFO *, INT, HTREEITEM, INT);
static VOID TREEVIEW_SetFirstVisible(TREEVIEW_INFO *, TREEVIEW_ITEM *, BOOL);
static LRESULT TREEVIEW_EnsureVisible(TREEVIEW_INFO *, HTREEITEM, BOOL);
static LRESULT TREEVIEW_EndEditLabelNow(TREEVIEW_INFO *infoPtr, BOOL bCancel);
static VOID TREEVIEW_UpdateScrollBars(TREEVIEW_INFO *infoPtr);
static LRESULT TREEVIEW_HScroll(TREEVIEW_INFO *, WPARAM);

/* Random Utilities *****************************************************/
static void TREEVIEW_VerifyTree(TREEVIEW_INFO *infoPtr);