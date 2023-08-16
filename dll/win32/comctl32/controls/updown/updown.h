/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     UpDown control header file
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "winnls.h"
#include "commctrl.h"
#include "comctl32.h"
#include "uxtheme.h"
#include "vssym32.h"
#include "wine/heap.h"
#include "wine/unicode.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(updown);

typedef struct
{
    HWND      Self;            /* Handle to this up-down control */
    HWND      Notify;          /* Handle to the parent window */
    DWORD     dwStyle;         /* The GWL_STYLE for this window */
    UINT      AccelCount;      /* Number of elements in AccelVect */
    UDACCEL*  AccelVect;       /* Vector containing AccelCount elements */
    INT       AccelIndex;      /* Current accel index, -1 if not accel'ing */
    INT       Base;            /* Base to display nr in the buddy window */
    INT       CurVal;          /* Current up-down value */
    INT       MinVal;          /* Minimum up-down value */
    INT       MaxVal;          /* Maximum up-down value */
    HWND      Buddy;           /* Handle to the buddy window */
    INT       BuddyType;       /* Remembers the buddy type BUDDY_TYPE_* */
    INT       Flags;           /* Internal Flags FLAG_* */
    BOOL      UnicodeFormat;   /* Marks the use of Unicode internally */
} UPDOWN_INFO;

/* Control configuration constants */

#define INITIAL_DELAY	500    /* initial timer until auto-inc kicks in */
#define AUTOPRESS_DELAY	250    /* time to keep arrow pressed on KEY_DOWN */
#define REPEAT_DELAY	50     /* delay between auto-increments */

#define DEFAULT_WIDTH	    16 /* default width of the ctrl */
#define DEFAULT_XSEP         0 /* default separation between buddy and ctrl */
#define DEFAULT_ADDTOP       0 /* amount to extend above the buddy window */
#define DEFAULT_ADDBOT       0 /* amount to extend below the buddy window */
#define DEFAULT_BUDDYBORDER  2 /* Width/height of the buddy border */
#define DEFAULT_BUDDYSPACER  2 /* Spacer between the buddy and the ctrl */
#define DEFAULT_BUDDYBORDER_THEMED  1 /* buddy border when theming is enabled */
#define DEFAULT_BUDDYSPACER_THEMED  0 /* buddy spacer when theming is enabled */

/* Work constants */

#define FLAG_INCR	0x01
#define FLAG_DECR	0x02
#define FLAG_MOUSEIN	0x04
#define FLAG_PRESSED	0x08
#define FLAG_BUDDYINT	0x10 /* UDS_SETBUDDYINT was set on creation */
#define FLAG_ARROW	(FLAG_INCR | FLAG_DECR)

#define BUDDY_TYPE_UNKNOWN 0
#define BUDDY_TYPE_LISTBOX 1
#define BUDDY_TYPE_EDIT    2

#define TIMER_AUTOREPEAT   1
#define TIMER_ACCEL        2
#define TIMER_AUTOPRESS    3

#define UPDOWN_GetInfoPtr(hwnd) ((UPDOWN_INFO *)GetWindowLongPtrW (hwnd,0))

/* id used for SetWindowSubclass */
#define BUDDY_SUBCLASSID   1