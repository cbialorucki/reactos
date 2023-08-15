/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Rebar control header file
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#pragma once
/*
 * Testing: set to 1 to make background brush *always* green
 */
#define GLATESTING 0

/*
 * 3. REBAR_MoveChildWindows should have a loop because more than
 *    one pass (together with the RBN_CHILDSIZEs) is made on
 *    at least RB_INSERTBAND
 */

#include <assert.h>
#include <stdarg.h>
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

WINE_DEFAULT_DEBUG_CHANNEL(rebar);

typedef struct
{
    UINT    fStyle;
    UINT    fMask;
    COLORREF  clrFore;
    COLORREF  clrBack;
    INT     iImage;
    HWND    hwndChild;
    UINT    cxMinChild;     /* valid if _CHILDSIZE */
    UINT    cyMinChild;     /* valid if _CHILDSIZE */
    UINT    cx;             /* valid if _SIZE */
    HBITMAP hbmBack;
    UINT    wID;
    UINT    cyChild;        /* valid if _CHILDSIZE */
    UINT    cyMaxChild;     /* valid if _CHILDSIZE */
    UINT    cyIntegral;     /* valid if _CHILDSIZE */
    UINT    cxIdeal;
    LPARAM    lParam;
    UINT    cxHeader;

    INT     cxEffective;    /* current cx for band */
    UINT    cyHeader;       /* the height of the header */
    UINT    cxMinBand;      /* minimum cx for band */
    UINT    cyMinBand;      /* minimum cy for band */

    UINT    cyRowSoFar;     /* for RBS_VARHEIGHT - the height of the row if it would break on this band (set by _Layout) */
    INT     iRow;           /* zero-based index of the row this band assigned to */
    UINT    fStatus;        /* status flags, reset only by _Validate */
    UINT    fDraw;          /* drawing flags, reset only by _Layout */
    UINT    uCDret;         /* last return from NM_CUSTOMDRAW */
    RECT    rcBand;         /* calculated band rectangle - coordinates swapped for CCS_VERT */
    RECT    rcGripper;      /* calculated gripper rectangle */
    RECT    rcCapImage;     /* calculated caption image rectangle */
    RECT    rcCapText;      /* calculated caption text rectangle */
    RECT    rcChild;        /* calculated child rectangle */
    RECT    rcChevron;      /* calculated chevron rectangle */

    LPWSTR    lpText;
    HWND    hwndPrevParent;
} REBAR_BAND;

/* has a value of: 0, CCS_TOP, CCS_NOMOVEY, CCS_BOTTOM */
#define CCS_LAYOUT_MASK 0x3

/* fStatus flags */
#define HAS_GRIPPER    0x00000001
#define HAS_IMAGE      0x00000002
#define HAS_TEXT       0x00000004

/* fDraw flags */
#define DRAW_GRIPPER    0x00000001
#define DRAW_IMAGE      0x00000002
#define DRAW_TEXT       0x00000004
#define DRAW_CHEVRONHOT 0x00000040
#define DRAW_CHEVRONPUSHED 0x00000080
#define NTF_INVALIDATE  0x01000000

typedef struct
{
    COLORREF   clrBk;       /* background color */
    COLORREF   clrText;     /* text color */
    COLORREF   clrBtnText;  /* system color for BTNTEXT */
    COLORREF   clrBtnFace;  /* system color for BTNFACE */
    HIMAGELIST himl;        /* handle to imagelist */
    UINT     uNumBands;   /* # of bands in rebar (first=0, last=uNumBands-1 */
    UINT     uNumRows;    /* # of rows of bands (first=1, last=uNumRows */
    HWND     hwndSelf;    /* handle of REBAR window itself */
    HWND     hwndToolTip; /* handle to the tool tip control */
    HWND     hwndNotify;  /* notification window (parent) */
    HFONT    hDefaultFont;
    HFONT    hFont;       /* handle to the rebar's font */
    SIZE     imageSize;   /* image size (image list) */
    DWORD    dwStyle;     /* window style */
    DWORD    orgStyle;    /* original style (dwStyle may change) */
    SIZE     calcSize;    /* calculated rebar size - coordinates swapped for CCS_VERT */
    BOOL     bUnicode;    /* TRUE if parent wants notify in W format */
    BOOL     DoRedraw;    /* TRUE to actually draw bands */
    UINT     fStatus;     /* Status flags (see below)  */
    HCURSOR  hcurArrow;   /* handle to the arrow cursor */
    HCURSOR  hcurHorz;    /* handle to the EW cursor */
    HCURSOR  hcurVert;    /* handle to the NS cursor */
    HCURSOR  hcurDrag;    /* handle to the drag cursor */
    INT      iVersion;    /* version number */
    POINT    dragStart;   /* x,y of button down */
    POINT    dragNow;     /* x,y of this MouseMove */
    INT      iOldBand;    /* last band that had the mouse cursor over it */
    INT      ihitoffset;  /* offset of hotspot from gripper.left */
    INT      ichevronhotBand; /* last band that had a hot chevron */
    INT      iGrabbedBand;/* band number of band whose gripper was grabbed */

    HDPA     bands;       /* pointer to the array of rebar bands */
} REBAR_INFO;

/* fStatus flags */
#define BEGIN_DRAG_ISSUED   0x00000001
#define SELF_RESIZE         0x00000002
#define BAND_NEEDS_REDRAW   0x00000020

/* used by Windows to mark that the header size has been set by the user and shouldn't be changed */
#define RBBS_UNDOC_FIXEDHEADER 0x40000000

/* ----   REBAR layout constants. Mostly determined by        ---- */
/* ----   experiment on WIN 98.                               ---- */

/* Width (or height) of separators between bands (either horz. or  */
/* vert.). True only if RBS_BANDBORDERS is set                     */
#define SEP_WIDTH_SIZE  2
#define SEP_WIDTH       ((infoPtr->dwStyle & RBS_BANDBORDERS) ? SEP_WIDTH_SIZE : 0)

/* Blank (background color) space between Gripper (if present)     */
/* and next item (image, text, or window). Always present          */
#define REBAR_ALWAYS_SPACE  4

/* Blank (background color) space after Image (if present).        */
#define REBAR_POST_IMAGE  2

/* Blank (background color) space after Text (if present).         */
#define REBAR_POST_TEXT  4

/* Height of vertical gripper in a CCS_VERT rebar.                 */
#define GRIPPER_HEIGHT  16

/* Blank (background color) space before Gripper (if present).     */
#define REBAR_PRE_GRIPPER   2

/* Width (of normal vertical gripper) or height (of horz. gripper) */
/* if present.                                                     */
#define GRIPPER_WIDTH  3

/* Width of the chevron button if present */
#define CHEVRON_WIDTH  10

/* the gap between the child and the next band */
#define REBAR_POST_CHILD 4

/* Height of divider for Rebar if not disabled (CCS_NODIVIDER)     */
/* either top or bottom                                            */
#define REBAR_DIVIDER  2

/* height of a rebar without a child */
#define REBAR_NO_CHILD_HEIGHT 4

/* minimum vertical height of a normal bar                        */
/*   or minimum width of a CCS_VERT bar - from experiment on Win2k */
#define REBAR_MINSIZE  23

/* This is the increment that is used over the band height         */
#define REBARSPACE(a)     ((a->fStyle & RBBS_CHILDEDGE) ? 2*REBAR_DIVIDER : 0)

/* ----   End of REBAR layout constants.                      ---- */

#define RB_GETBANDINFO_OLD (WM_USER+5) /* obsoleted after IE3, but we have to support it anyway */

/*  The following define determines if a given band is hidden      */
#define HIDDENBAND(a)  (((a)->fStyle & RBBS_HIDDEN) ||   \
                        ((infoPtr->dwStyle & CCS_VERT) &&         \
                         ((a)->fStyle & RBBS_NOVERT)))

#define REBAR_GetInfoPtr(wndPtr) ((REBAR_INFO *)GetWindowLongPtrW (hwnd, 0))

static LRESULT REBAR_NotifyFormat(REBAR_INFO *infoPtr, LPARAM lParam);
static void REBAR_AutoSize(REBAR_INFO *infoPtr, BOOL needsLayout);

/* no index check here */
static inline REBAR_BAND* REBAR_GetBand(const REBAR_INFO *infoPtr, INT i)
{
    assert(i >= 0 && (UINT)i < infoPtr->uNumBands);
    return (REBAR_BAND*)DPA_GetPtr(infoPtr->bands, i);
}

/* "constant values" retrieved when DLL was initialized    */
/* FIXME we do this when the classes are registered.       */
static UINT mindragx = 0;
static UINT mindragy = 0;

static const char * const band_stylename[] = {
    "RBBS_BREAK",              /* 0001 */
    "RBBS_FIXEDSIZE",          /* 0002 */
    "RBBS_CHILDEDGE",          /* 0004 */
    "RBBS_HIDDEN",             /* 0008 */
    "RBBS_NOVERT",             /* 0010 */
    "RBBS_FIXEDBMP",           /* 0020 */
    "RBBS_VARIABLEHEIGHT",     /* 0040 */
    "RBBS_GRIPPERALWAYS",      /* 0080 */
    "RBBS_NOGRIPPER",          /* 0100 */
    NULL };

static const char * const band_maskname[] = {
    "RBBIM_STYLE",         /*    0x00000001 */
    "RBBIM_COLORS",        /*    0x00000002 */
    "RBBIM_TEXT",          /*    0x00000004 */
    "RBBIM_IMAGE",         /*    0x00000008 */
    "RBBIM_CHILD",         /*    0x00000010 */
    "RBBIM_CHILDSIZE",     /*    0x00000020 */
    "RBBIM_SIZE",          /*    0x00000040 */
    "RBBIM_BACKGROUND",    /*    0x00000080 */
    "RBBIM_ID",            /*    0x00000100 */
    "RBBIM_IDEALSIZE",     /*    0x00000200 */
    "RBBIM_LPARAM",        /*    0x00000400 */
    "RBBIM_HEADERSIZE",    /*    0x00000800 */
    NULL };

static CHAR line[200];

static const WCHAR themeClass[] = L"Rebar";