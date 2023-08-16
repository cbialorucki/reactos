#pragma once

#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "winnls.h"
#include "commctrl.h"
#include "comctl32.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(datetime);

typedef struct
{
    HWND hwndSelf;
    HWND hMonthCal;
    HWND hwndNotify;
    HWND hUpdown;
    DWORD dwStyle;
    SYSTEMTIME date;
    BOOL dateValid;
    HWND hwndCheckbut;
    RECT rcClient; /* rect around the edge of the window */
    RECT rcDraw; /* rect inside of the border */
    RECT checkbox;  /* checkbox allowing the control to be enabled/disabled */
    RECT calbutton; /* button that toggles the dropdown of the monthcal control */
    BOOL bCalDepressed; /* TRUE = cal button is depressed */
    BOOL bDropdownEnabled;
    int  select;
    WCHAR charsEntered[4];
    int nCharsEntered;
    HFONT hFont;
    int nrFieldsAllocated;
    int nrFields;
    int haveFocus;
    int *fieldspec;
    RECT *fieldRect;
    int  *buflen;
    WCHAR textbuf[256];
    POINT monthcal_pos;
    int pendingUpdown;
} DATETIME_INFO, *LPDATETIME_INFO;

/* in monthcal.c */
extern int MONTHCAL_MonthLength(int month, int year);
extern int MONTHCAL_CalculateDayOfWeek(SYSTEMTIME *date, BOOL inplace);

/* this list of defines is closely related to `allowedformatchars' defined
 * in datetime.c; the high nibble indicates the `base type' of the format
 * specifier.
 * Do not change without first reading DATETIME_UseFormat.
 *
 */

#define DT_END_FORMAT      0
#define ONEDIGITDAY   	0x01
#define TWODIGITDAY   	0x02
#define THREECHARDAY  	0x03
#define FULLDAY         0x04
#define ONEDIGIT12HOUR  0x11
#define TWODIGIT12HOUR  0x12
#define ONEDIGIT24HOUR  0x21
#define TWODIGIT24HOUR  0x22
#define ONEDIGITMINUTE  0x31
#define TWODIGITMINUTE  0x32
#define ONEDIGITMONTH   0x41
#define TWODIGITMONTH   0x42
#define THREECHARMONTH  0x43
#define FULLMONTH       0x44
#define ONEDIGITSECOND  0x51
#define TWODIGITSECOND  0x52
#define ONELETTERAMPM   0x61
#define TWOLETTERAMPM   0x62
#define ONEDIGITYEAR    0x71
#define TWODIGITYEAR    0x72
#define INVALIDFULLYEAR 0x73      /* FIXME - yyy is not valid - we'll treat it as yyyy */
#define FULLYEAR        0x74
#define FORMATCALLBACK  0x81      /* -> maximum of 0x80 callbacks possible */
#define FORMATCALLMASK  0x80
#define DT_STRING 	0x0100

#define DTHT_DATEFIELD  0xff      /* for hit-testing */

#define DTHT_NONE       0x1000
#define DTHT_CHECKBOX   0x2000  /* these should end at '00' , to make */
#define DTHT_MCPOPUP    0x3000  /* & DTHT_DATEFIELD 0 when DATETIME_KeyDown */
#define DTHT_GOTFOCUS   0x4000  /* tests for date-fields */
#define DTHT_NODATEMASK 0xf000  /* to mask check and drop down from others */

static BOOL DATETIME_SendSimpleNotify (const DATETIME_INFO *infoPtr, UINT code);
static BOOL DATETIME_SendDateTimeChangeNotify (const DATETIME_INFO *infoPtr);
static const WCHAR allowedformatchars[] = L"dhHmMstyX";
static const int maxrepetition [] = {4,2,2,2,4,2,2,4,-1};

/* valid date limits */
static const SYSTEMTIME max_allowed_date = { /*.wYear =*/ 9999, /*.wMonth =*/ 12, /*.wDayOfWeek =*/ 0, /*.wDay =*/ 31 };
static const SYSTEMTIME min_allowed_date = { /*.wYear =*/ 1752, /*.wMonth =*/ 9, /*.wDayOfWeek =*/ 0, /*.wDay =*/ 14 };