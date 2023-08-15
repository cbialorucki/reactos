/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     SysLink control header file
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#pragma once

#include <stdarg.h>
#include <string.h>
#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "winnls.h"
#include "commctrl.h"
#include "comctl32.h"
#include "wine/unicode.h"
#include "wine/debug.h"
#include "wine/list.h"

WINE_DEFAULT_DEBUG_CHANNEL(syslink);

typedef struct
{
    int nChars;
    int nSkip;
    RECT rc;
} DOC_TEXTBLOCK, *PDOC_TEXTBLOCK;

#define LIF_FLAGSMASK   (LIF_STATE | LIF_ITEMID | LIF_URL)
#define LIS_MASK        (LIS_FOCUSED | LIS_ENABLED | LIS_VISITED)

typedef enum
{
    slText = 0,
    slLink
} SL_ITEM_TYPE;

typedef struct _DOC_ITEM
{
    struct list entry;
    UINT nText;             /* Number of characters of the text */
    SL_ITEM_TYPE Type;      /* type of the item */
    PDOC_TEXTBLOCK Blocks;  /* Array of text blocks */
    union
    {
        struct
        {
            UINT state;     /* Link state */
            WCHAR *szID;    /* Link ID string */
            WCHAR *szUrl;   /* Link URL string */
        } Link;
        struct
        {
            UINT Dummy;
        } Text;
    } u;
    WCHAR Text[1];          /* Text of the document item */
} DOC_ITEM, *PDOC_ITEM;

typedef struct
{
    HWND      Self;         /* The window handle for this control */
    HWND      Notify;       /* The parent handle to receive notifications */
    DWORD     Style;        /* Styles for this control */
    struct list Items;      /* Document items list */
    BOOL      HasFocus;     /* Whether the control has the input focus */
    int       MouseDownID;  /* ID of the link that the mouse button first selected */
    HFONT     Font;         /* Handle to the font for text */
    HFONT     LinkFont;     /* Handle to the font for links */
    COLORREF  TextColor;    /* Color of the text */
    COLORREF  LinkColor;    /* Color of links */
    COLORREF  VisitedColor; /* Color of visited links */
    WCHAR     BreakChar;    /* Break Character for the current font */
    BOOL      IgnoreReturn; /* (infoPtr->Style & LWS_IGNORERETURN) on creation */
} SYSLINK_INFO;

/* Control configuration constants */

#define SL_LEFTMARGIN   (0)
#define SL_TOPMARGIN    (0)
#define SL_RIGHTMARGIN  (0)
#define SL_BOTTOMMARGIN (0)