/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Month calendar control header file
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#pragma once

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
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
#include "wine/unicode.h"
#include "wine/debug.h"
#include "wine/heap.h"

WINE_DEFAULT_DEBUG_CHANNEL(monthcal);

/* FIXME: Inspect */

#define MC_SEL_LBUTUP	    1	/* Left button released */
#define MC_SEL_LBUTDOWN	    2	/* Left button pressed in calendar */
#define MC_PREVPRESSED      4   /* Prev month button pressed */
#define MC_NEXTPRESSED      8   /* Next month button pressed */
#define MC_PREVNEXTMONTHDELAY   350	/* when continuously pressing `next/prev month', wait 350 ms before going to the next/prev month */
#define MC_TODAYUPDATEDELAY 120000 /* time between today check for update (2 min) */

#define MC_PREVNEXTMONTHTIMER   1	/* Timer IDs */
#define MC_TODAYUPDATETIMER     2

#define MC_CALENDAR_PADDING     6

/* convert from days to 100 nanoseconds unit - used as FILETIME unit */
#define DAYSTO100NSECS(days) (((ULONGLONG)(days))*24*60*60*10000000)

enum CachedPen
{
    PenRed = 0,
    PenText,
    PenLast
};

enum CachedBrush
{
    BrushTitle = 0,
    BrushMonth,
    BrushBackground,
    BrushLast
};

/* single calendar data */
typedef struct _CALENDAR_INFO
{
    RECT title;      /* rect for the header above the calendar */
    RECT titlemonth; /* the 'month name' text in the header */
    RECT titleyear;  /* the 'year number' text in the header */
    RECT wdays;      /* week days at top */
    RECT days;       /* calendar area */
    RECT weeknums;   /* week numbers at left side */

    SYSTEMTIME month;/* contains calendar main month/year */
} CALENDAR_INFO;

typedef struct
{
    HWND	m_hwnd;
    DWORD	dwStyle; /* cached GWL_STYLE */

    COLORREF    colors[MCSC_TRAILINGTEXT+1];
    HBRUSH      brushes[BrushLast];
    HPEN        pens[PenLast];

    HFONT	hFont;
    HFONT	hBoldFont;
    int		textHeight;
    int		height_increment;
    int		width_increment;
    INT		delta;	/* scroll rate; # of months that the */
                        /* control moves when user clicks a scroll button */
    int		firstDay;	/* Start month calendar with firstDay's day,
				   stored in SYSTEMTIME format */
    BOOL	firstDaySet;    /* first week day differs from locale defined */

    BOOL	isUnicode;      /* value set with MCM_SETUNICODE format */

    MONTHDAYSTATE *monthdayState;
    SYSTEMTIME	todaysDate;
    BOOL	todaySet;       /* Today was forced with MCM_SETTODAY */
    int		status;		/* See MC_SEL flags */
    SYSTEMTIME	firstSel;	/* first selected day */
    INT		maxSelCount;
    SYSTEMTIME	minSel;         /* contains single selection when used without MCS_MULTISELECT */
    SYSTEMTIME	maxSel;
    SYSTEMTIME  focusedSel;     /* date currently focused with mouse movement */
    DWORD	rangeValid;
    SYSTEMTIME	minDate;
    SYSTEMTIME	maxDate;

    RECT titlebtnnext;	/* the `next month' button in the header */
    RECT titlebtnprev;  /* the `prev month' button in the header */
    RECT todayrect;	/* `today: xx/xx/xx' text rect */
    HWND hwndNotify;    /* Window to receive the notifications */
    HWND hWndYearEdit;  /* Window Handle of edit box to handle years */
    HWND hWndYearUpDown;/* Window Handle of updown box to handle years */
    WNDPROC EditWndProc;  /* original Edit window procedure */

    CALENDAR_INFO *calendars;
    SIZE dim;           /* [cx,cy] - dimensions of calendars matrix, row/column count */
} MONTHCAL_INFO, *LPMONTHCAL_INFO;

static const WCHAR themeClass[] = L"Scrollbar";

/* empty SYSTEMTIME const */
static const SYSTEMTIME st_null = SYSTEMTIME();
/* valid date limits */
static const SYSTEMTIME max_allowed_date = { /* wYear */ 9999, /* wMonth */ 12, /* wDayOfWeek */ 0, /* wDay */ 31 };
static const SYSTEMTIME min_allowed_date = { /* wYear */ 1752, /* wMonth */ 9,  /* wDayOfWeek */ 0, /* wDay */ 14 };

/* Prev/Next buttons */
enum nav_direction
{
    DIRECTION_BACKWARD,
    DIRECTION_FORWARD
};