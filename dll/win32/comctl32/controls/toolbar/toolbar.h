/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Toolbar control header file
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#pragma once

#include <stdarg.h>
#include <string.h>

#include "windef.h"
#include "winbase.h"
#include "winreg.h"
#include "wingdi.h"
#include "winuser.h"
#include "wine/unicode.h"
#include "winnls.h"
#include "commctrl.h"
#include "comctl32.h"
#include "uxtheme.h"
#include "vssym32.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(toolbar);

static HCURSOR hCursorDrag = NULL;

typedef struct
{
    INT iBitmap;
    INT idCommand;
    BYTE  fsState;
    BYTE  fsStyle;
    BOOL  bHot;
    BOOL  bDropDownPressed;
    DWORD_PTR dwData;
    INT_PTR iString;
    INT nRow;
    RECT rect;
    INT cx; /* manually set size */
} TBUTTON_INFO;

typedef struct
{
    UINT nButtons;
    HINSTANCE hInst;
    UINT nID;
} TBITMAP_INFO;

typedef struct
{
    HIMAGELIST himl;
    INT id;
} IMLENTRY, *PIMLENTRY;

typedef struct
{
    DWORD    dwStructSize;    /* size of TBBUTTON struct */
    RECT     client_rect;
    RECT     rcBound;         /* bounding rectangle */
    INT      nButtonHeight;
    INT      nButtonWidth;
    INT      nBitmapHeight;
    INT      nBitmapWidth;
    INT      nIndent;
    INT      nRows;           /* number of button rows */
    INT      nMaxTextRows;    /* maximum number of text rows */
    INT      cxMin;           /* minimum button width */
    INT      cxMax;           /* maximum button width */
    INT      nNumButtons;     /* number of buttons */
    INT      nNumBitmaps;     /* number of bitmaps */
    INT      nNumStrings;     /* number of strings */
    INT      nNumBitmapInfos;
    INT      nButtonDown;     /* toolbar button being pressed or -1 if none */
    INT      nButtonDrag;     /* toolbar button being dragged or -1 if none */
    INT      nOldHit;
    INT      nHotItem;        /* index of the "hot" item */
    SIZE     szPadding;       /* padding values around button */
    SIZE     szBarPadding;       /* padding values around the toolbar (NOT USED BUT STORED) */
    SIZE     szSpacing;       /* spacing values between buttons */
    MARGINS  themeMargins;
    INT      iTopMargin;      /* the top margin */
    INT      iListGap;        /* default gap between text and image for toolbar with list style */
    HFONT    hDefaultFont;
    HFONT    hFont;           /* text font */
    HIMAGELIST himlInt;       /* image list created internally */
    PIMLENTRY *himlDef;       /* default image list array */
    INT       cimlDef;        /* default image list array count */
    PIMLENTRY *himlHot;       /* hot image list array */
    INT       cimlHot;        /* hot image list array count */
    PIMLENTRY *himlDis;       /* disabled image list array */
    INT       cimlDis;        /* disabled image list array count */
    HWND     hwndToolTip;     /* handle to tool tip control */
    HWND     hwndNotify;      /* handle to the window that gets notifications */
    HWND     hwndSelf;        /* my own handle */
    BOOL     bAnchor;         /* anchor highlight enabled */
    BOOL     bDoRedraw;       /* Redraw status */
    BOOL     bDragOutSent;    /* has TBN_DRAGOUT notification been sent for this drag? */
    BOOL     bUnicode;        /* Notifications are ASCII (FALSE) or Unicode (TRUE)? */
    BOOL     bCaptured;       /* mouse captured? */
    DWORD      dwStyle;       /* regular toolbar style */
    DWORD      dwExStyle;     /* extended toolbar style */
    DWORD      dwDTFlags;     /* DrawText flags */

    COLORREF   clrInsertMark;   /* insert mark color */
    COLORREF   clrBtnHighlight; /* color for Flat Separator */
    COLORREF   clrBtnShadow;    /* color for Flag Separator */
    INT      iVersion;
    LPWSTR   pszTooltipText;    /* temporary store for a string > 80 characters
                                 * for TTN_GETDISPINFOW notification */
    TBINSERTMARK  tbim;         /* info on insertion mark */
    TBUTTON_INFO *buttons;      /* pointer to button array */
    LPWSTR       *strings;      /* pointer to string array */
    TBITMAP_INFO *bitmaps;
} TOOLBAR_INFO, *PTOOLBAR_INFO;


/* used by customization dialog */
typedef struct
{
    PTOOLBAR_INFO tbInfo;
    HWND          tbHwnd;
} CUSTDLG_INFO, *PCUSTDLG_INFO;

typedef struct
{
    TBBUTTON btn;
    BOOL     bVirtual;
    BOOL     bRemovable;
    WCHAR    text[64];
} CUSTOMBUTTON, *PCUSTOMBUTTON;

typedef enum
{
    IMAGE_LIST_DEFAULT,
    IMAGE_LIST_HOT,
    IMAGE_LIST_DISABLED
} IMAGE_LIST_TYPE;

#define SEPARATOR_WIDTH    8
#define TOP_BORDER         2
#define BOTTOM_BORDER      2
#define DDARROW_WIDTH      11
#define ARROW_HEIGHT       3
#define INSERTMARK_WIDTH   2

/* default padding inside a button */
#define DEFPAD_CX 7
#define DEFPAD_CY 6

/* default space between buttons and between rows */
#define DEFSPACE_CX 7
#define DEFSPACE_CY 6

#define DEFLISTGAP 4

/* vertical padding used in list mode when image is present */
#ifdef __REACTOS__
#define LISTPAD_CY 2
#else
#define LISTPAD_CY 9
#endif

/* how wide to treat the bitmap if it isn't present */
#define NONLIST_NOTEXT_OFFSET 2

#define TOOLBAR_NOWHERE (-1)

/* Used to find undocumented extended styles */
#define TBSTYLE_EX_ALL (TBSTYLE_EX_DRAWDDARROWS | \
                        TBSTYLE_EX_VERTICAL | \
                        TBSTYLE_EX_MIXEDBUTTONS | \
                        TBSTYLE_EX_DOUBLEBUFFER | \
                        TBSTYLE_EX_HIDECLIPPEDBUTTONS)

/* all of the CCS_ styles */
#define COMMON_STYLES (CCS_TOP|CCS_NOMOVEY|CCS_BOTTOM|CCS_NORESIZE| \
                       CCS_NOPARENTALIGN|CCS_ADJUSTABLE|CCS_NODIVIDER|CCS_VERT)

#define GETIBITMAP(infoPtr, i) (infoPtr->iVersion >= 5 ? LOWORD(i) : i)
#define GETHIMLID(infoPtr, i) (infoPtr->iVersion >= 5 ? HIWORD(i) : 0)
#define GETDEFIMAGELIST(infoPtr, id) TOOLBAR_GetImageList(infoPtr->himlDef, infoPtr->cimlDef, id)
#define GETHOTIMAGELIST(infoPtr, id) TOOLBAR_GetImageList(infoPtr->himlHot, infoPtr->cimlHot, id)
#define GETDISIMAGELIST(infoPtr, id) TOOLBAR_GetImageList(infoPtr->himlDis, infoPtr->cimlDis, id)

static const WCHAR themeClass[] = { 'T','o','o','l','b','a','r',0 };

static BOOL TOOLBAR_GetButtonInfo(const TOOLBAR_INFO *infoPtr, NMTOOLBARW *nmtb);
static BOOL TOOLBAR_IsButtonRemovable(const TOOLBAR_INFO *infoPtr, int iItem, const CUSTOMBUTTON *btnInfo);
static HIMAGELIST TOOLBAR_GetImageList(const PIMLENTRY *pies, INT cies, INT id);
static PIMLENTRY TOOLBAR_GetImageListEntry(const PIMLENTRY *pies, INT cies, INT id);
static VOID TOOLBAR_DeleteImageList(PIMLENTRY **pies, INT *cies);
static HIMAGELIST TOOLBAR_InsertImageList(PIMLENTRY **pies, INT *cies, HIMAGELIST himl, INT id);
static LRESULT TOOLBAR_LButtonDown(TOOLBAR_INFO *infoPtr, WPARAM wParam, LPARAM lParam);
static void TOOLBAR_LayoutToolbar(TOOLBAR_INFO *infoPtr);
static LRESULT TOOLBAR_AutoSize(TOOLBAR_INFO *infoPtr);
static void TOOLBAR_CheckImageListIconSize(TOOLBAR_INFO *infoPtr);
static void TOOLBAR_TooltipAddTool(const TOOLBAR_INFO *infoPtr, const TBUTTON_INFO *button);
static void TOOLBAR_TooltipSetRect(const TOOLBAR_INFO *infoPtr, const TBUTTON_INFO *button);
static LRESULT TOOLBAR_SetButtonInfo(TOOLBAR_INFO *infoPtr, INT Id,
                                     const TBBUTTONINFOW *lptbbi, BOOL isW);
