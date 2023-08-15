/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Listbox control header file
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "commctrl.h"
#include "uxtheme.h"
#include "vssym32.h"
#include "wine/unicode.h"
#include "wine/exception.h"
#include "wine/debug.h"

#include "comctl32.h"

WINE_DEFAULT_DEBUG_CHANNEL(listbox2);

/* Items array granularity */
#define LB_ARRAY_GRANULARITY 16

/* Scrolling timeout in ms */
#define LB_SCROLL_TIMEOUT 50

/* Listbox system timer id */
#define LB_TIMER_ID  2

/* flag listbox changed while setredraw false - internal style */
#define LBS_DISPLAYCHANGED 0x80000000

/* Item structure */
typedef struct
{
    LPWSTR    str;       /* Item text */
    BOOL      selected;  /* Is item selected? */
    UINT      height;    /* Item height (only for OWNERDRAWVARIABLE) */
    ULONG_PTR data;      /* User data */
} LB_ITEMDATA;

/* Listbox structure */
typedef struct
{
    HWND        self;           /* Our own window handle */
    HWND        owner;          /* Owner window to send notifications to */
    UINT        style;          /* Window style */
    INT         width;          /* Window width */
    INT         height;         /* Window height */
    LB_ITEMDATA  *items;        /* Array of items */
    INT         nb_items;       /* Number of items */
    INT         top_item;       /* Top visible item */
    INT         selected_item;  /* Selected item */
    INT         focus_item;     /* Item that has the focus */
    INT         anchor_item;    /* Anchor item for extended selection */
    INT         item_height;    /* Default item height */
    INT         page_size;      /* Items per listbox page */
    INT         column_width;   /* Column width for multi-column listboxes */
    INT         horz_extent;    /* Horizontal extent */
    INT         horz_pos;       /* Horizontal position */
    INT         nb_tabs;        /* Number of tabs in array */
    INT        *tabs;           /* Array of tabs */
    INT         avg_char_width; /* Average width of characters */
    INT         wheel_remain;   /* Left over scroll amount */
    BOOL        caret_on;       /* Is caret on? */
    BOOL        captured;       /* Is mouse captured? */
    BOOL        in_focus;
    HFONT       font;           /* Current font */
    LCID        locale;         /* Current locale for string comparisons */
    HEADCOMBO  *lphc;           /* ComboLBox */
} LB_DESCR;


#define IS_OWNERDRAW(descr) \
    ((descr)->style & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE))

#define HAS_STRINGS(descr) \
    (!IS_OWNERDRAW(descr) || ((descr)->style & LBS_HASSTRINGS))


#define IS_MULTISELECT(descr) \
    ((descr)->style & (LBS_MULTIPLESEL|LBS_EXTENDEDSEL) && \
     !((descr)->style & LBS_NOSEL))

#define SEND_NOTIFICATION(descr,code) \
    (SendMessageW( (descr)->owner, WM_COMMAND, \
     MAKEWPARAM( GetWindowLongPtrW((descr->self),GWLP_ID), (code)), (LPARAM)(descr->self) ))

#define ISWIN31 (LOWORD(GetVersion()) == 0x0a03)

/* Current timer status */
typedef enum
{
    LB_TIMER_NONE,
    LB_TIMER_UP,
    LB_TIMER_LEFT,
    LB_TIMER_DOWN,
    LB_TIMER_RIGHT
} TIMER_DIRECTION;

static TIMER_DIRECTION LISTBOX_Timer = LB_TIMER_NONE;

static LRESULT LISTBOX_GetItemRect( const LB_DESCR *descr, INT index, RECT *rect );