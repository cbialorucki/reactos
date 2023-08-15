/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Property sheet control header file
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
#include "prsht.h"
#include "comctl32.h"
#include "uxtheme.h"

#include "pshpack2.h"

typedef struct
{
  WORD dlgVer;
  WORD signature;
  DWORD helpID;
  DWORD exStyle;
  DWORD style;
} MyDLGTEMPLATEEX;

typedef struct
{
  DWORD helpid;
  DWORD exStyle;
  DWORD style;
  short x;
  short y;
  short cx;
  short cy;
  DWORD id;
} MyDLGITEMTEMPLATEEX;

#include "poppack.h"

typedef struct tagPropPageInfo
{
  HPROPSHEETPAGE hpage; /* to keep track of pages not passed to PropertySheet */
  HWND hwndPage;
  BOOL isDirty;
  LPCWSTR pszText;
  BOOL hasHelp;
  BOOL useCallback;
  BOOL hasIcon;
} PropPageInfo;

typedef struct tagPropSheetInfo
{
  HWND hwnd;
  PROPSHEETHEADERW ppshheader;
  BOOL unicode;
  LPWSTR strPropertiesFor;
  int nPages;
  int active_page;
  BOOL isModeless;
  BOOL hasHelp;
  BOOL hasApply;
  BOOL hasFinish;
  BOOL usePropPage;
  BOOL useCallback;
  BOOL activeValid;
  PropPageInfo* proppage;
  HFONT hFont;
  HFONT hFontBold;
  int width;
  int height;
  HIMAGELIST hImageList;
  BOOL ended;
  INT result;
} PropSheetInfo;

typedef struct
{
  int x;
  int y;
} PADDING_INFO;

/******************************************************************************
 * Defines and global variables
 */

static const WCHAR PropSheetInfoStr[] = L"PropertySheetInfo";

#define PSP_INTERNAL_UNICODE 0x80000000

#define MAX_CAPTION_LENGTH 255
#define MAX_TABTEXT_LENGTH 255
#define MAX_BUTTONTEXT_LENGTH 64

#define INTRNL_ANY_WIZARD (PSH_WIZARD | PSH_WIZARD97_OLD | PSH_WIZARD97_NEW | PSH_WIZARD_LITE)

/* Wizard metrics specified in DLUs */
#define WIZARD_PADDING 7
#define WIZARD_HEADER_HEIGHT 36

/******************************************************************************
 * Prototypes
 */
static PADDING_INFO PROPSHEET_GetPaddingInfo(HWND hwndDlg);
static void PROPSHEET_SetTitleW(HWND hwndDlg, DWORD dwStyle, LPCWSTR lpszText);
static BOOL PROPSHEET_CanSetCurSel(HWND hwndDlg);
static BOOL PROPSHEET_SetCurSel(HWND hwndDlg, int index, int skipdir, HPROPSHEETPAGE hpage);
static int PROPSHEET_GetPageIndex(HPROPSHEETPAGE hpage, const PropSheetInfo* psInfo, int original_index);
static PADDING_INFO PROPSHEET_GetPaddingInfoWizard(HWND hwndDlg, const PropSheetInfo* psInfo);
static BOOL PROPSHEET_DoCommand(HWND hwnd, WORD wID);
static BOOL PROPSHEET_RemovePage(HWND hwndDlg, int index, HPROPSHEETPAGE hpage);

static INT_PTR CALLBACK PROPSHEET_DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);