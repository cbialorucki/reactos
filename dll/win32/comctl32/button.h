/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Button control header file
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "uxtheme.h"

#define MAX_BTN_TYPE 16

/* Undocumented Flags */
#define BUTTON_NSTATES 0x0F
#define BUTTON_BTNPRESSED 0x40
#define BUTTON_UNKNOWN2 0x20
#define BUTTON_UNKNOWN3 0x10
#define BUTTON_BMCLICK 0x100

#define BUTTON_NOTIFY_PARENT(hWnd, code) \
    do { /* Notify parent which has created this button control */ \
        SendMessageW(GetParent(hWnd), WM_COMMAND, \
                     MAKEWPARAM(GetWindowLongPtrW((hWnd),GWLP_ID), (code)), \
                     (LPARAM)(hWnd)); \
    } while(0)

typedef struct _BUTTON_INFO
{
    HWND        hwnd;
    LONG        state;
    HFONT       font;
    union
    {
        HICON   icon;
        HBITMAP bitmap;
        HANDLE  image;
    } u;

    DWORD ui_state;
    RECT rcTextMargin;
    BUTTON_IMAGELIST imlData;
} BUTTON_INFO;

static const WORD maxCheckState[MAX_BTN_TYPE] =
{
    BST_UNCHECKED,      /* BS_PUSHBUTTON */
    BST_UNCHECKED,      /* BS_DEFPUSHBUTTON */
    BST_CHECKED,        /* BS_CHECKBOX */
    BST_CHECKED,        /* BS_AUTOCHECKBOX */
    BST_CHECKED,        /* BS_RADIOBUTTON */
    BST_INDETERMINATE,  /* BS_3STATE */
    BST_INDETERMINATE,  /* BS_AUTO3STATE */
    BST_UNCHECKED,      /* BS_GROUPBOX */
    BST_UNCHECKED,      /* BS_USERBUTTON */
    BST_CHECKED,        /* BS_AUTORADIOBUTTON */
    BST_UNCHECKED,      /* BS_PUSHBOX */
    BST_UNCHECKED,      /* BS_OWNERDRAW */
    BST_UNCHECKED,      /* BS_SPLITBUTTON */
    BST_UNCHECKED,      /* BS_DEFSPLITBUTTON */
    BST_UNCHECKED,      /* BS_COMMANDLINK */
    BST_UNCHECKED       /* BS_DEFCOMMANDLINK */
};

/* These are indices into a states array to determine the theme state for a given theme part. */
typedef enum
{
    STATE_NORMAL,
    STATE_DISABLED,
    STATE_HOT,
    STATE_PRESSED,
    STATE_DEFAULTED
} ButtonState;

typedef void (*pfPaint)(const BUTTON_INFO *infoPtr, HDC hdc, UINT action);
typedef void (*pfThemedPaint)( HTHEME theme, const BUTTON_INFO *infoPtr, HDC hdc, ButtonState drawState, UINT dtflags, BOOL focused, LPARAM prfFlag);

static UINT BUTTON_CalcLabelRect(const BUTTON_INFO *infoPtr, HDC hdc, RECT *rc);
static void PB_Paint(const BUTTON_INFO *infoPtr, HDC hDC, UINT action);
static void CB_Paint(const BUTTON_INFO *infoPtr, HDC hDC, UINT action);
static void GB_Paint(const BUTTON_INFO *infoPtr, HDC hDC, UINT action);
static void UB_Paint(const BUTTON_INFO *infoPtr, HDC hDC, UINT action);
static void OB_Paint(const BUTTON_INFO *infoPtr, HDC hDC, UINT action);
static void BUTTON_CheckAutoRadioButton(HWND hwnd);

static void PB_ThemedPaint(HTHEME theme, const BUTTON_INFO *infoPtr, HDC hdc, ButtonState drawState, UINT dtflags, BOOL focused, LPARAM prfFlag);
static void CB_ThemedPaint(HTHEME theme, const BUTTON_INFO *infoPtr, HDC hdc, ButtonState drawState, UINT dtflags, BOOL focused, LPARAM prfFlag);
static void GB_ThemedPaint(HTHEME theme, const BUTTON_INFO *infoPtr, HDC hdc, ButtonState drawState, UINT dtflags, BOOL focused, LPARAM prfFlag);

static const pfPaint btnPaintFunc[MAX_BTN_TYPE] =
{
    PB_Paint,    /* BS_PUSHBUTTON */
    PB_Paint,    /* BS_DEFPUSHBUTTON */
    CB_Paint,    /* BS_CHECKBOX */
    CB_Paint,    /* BS_AUTOCHECKBOX */
    CB_Paint,    /* BS_RADIOBUTTON */
    CB_Paint,    /* BS_3STATE */
    CB_Paint,    /* BS_AUTO3STATE */
    GB_Paint,    /* BS_GROUPBOX */
    UB_Paint,    /* BS_USERBUTTON */
    CB_Paint,    /* BS_AUTORADIOBUTTON */
    NULL,        /* BS_PUSHBOX */
    OB_Paint,    /* BS_OWNERDRAW */
    PB_Paint,    /* BS_SPLITBUTTON */
    PB_Paint,    /* BS_DEFSPLITBUTTON */
    PB_Paint,    /* BS_COMMANDLINK */
    PB_Paint     /* BS_DEFCOMMANDLINK */
};

static const pfThemedPaint btnThemedPaintFunc[MAX_BTN_TYPE] =
{
    PB_ThemedPaint, /* BS_PUSHBUTTON */
    PB_ThemedPaint, /* BS_DEFPUSHBUTTON */
    CB_ThemedPaint, /* BS_CHECKBOX */
    CB_ThemedPaint, /* BS_AUTOCHECKBOX */
    CB_ThemedPaint, /* BS_RADIOBUTTON */
    CB_ThemedPaint, /* BS_3STATE */
    CB_ThemedPaint, /* BS_AUTO3STATE */
    GB_ThemedPaint, /* BS_GROUPBOX */
    NULL,           /* BS_USERBUTTON */
    CB_ThemedPaint, /* BS_AUTORADIOBUTTON */
    NULL,           /* BS_PUSHBOX */
    NULL,           /* BS_OWNERDRAW */
    NULL,           /* BS_SPLITBUTTON */
    NULL,           /* BS_DEFSPLITBUTTON */
    NULL,           /* BS_COMMANDLINK */
    NULL,           /* BS_DEFCOMMANDLINK */
};