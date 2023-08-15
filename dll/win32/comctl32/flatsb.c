/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Flat scrollbar control
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
/* NOTES:
 *  This is a stub.
 */

#include <stdarg.h>
#include <string.h>
#include "windef.h"
#include "winbase.h"
#include "winerror.h"
#include "winuser.h"
#include "commctrl.h"
#include "comctl32.h"

static LPCWSTR ClassName = L"flatsb_class32";

/***********************************************************************
 *		InitializeFlatSB (COMCTL32.@)
 *
 * Initializes flat scroll bars for the specified window.
 *
 * RETURNS
 *     Success: Non-zero
 *     Failure: Zero
 *
 * NOTES
 *     Subclasses specified window so that flat scroll bars may be drawn
 *     and used.
 */
BOOL WINAPI InitializeFlatSB(HWND hwnd)
{
    return TRUE;
}

/***********************************************************************
 *		UninitializeFlatSB (COMCTL32.@)
 *
 * Uninitializes flat scroll bars for the specified window.
 *
 * RETURNS
 *	E_FAIL		if one of the scroll bars is currently in use
 *	S_FALSE		if InitializeFlatSB() was never called on this hwnd
 *	S_OK		otherwise
 *
 * NOTES
 *     Removes any subclassing on the specified window so that regular
 *     scroll bars are drawn and used.
 */
HRESULT WINAPI UninitializeFlatSB(HWND hwnd)
{
    return S_FALSE;
}

/***********************************************************************
 *		FlatSB_GetScrollProp (COMCTL32.@)
 *
 * Retrieves flat-scroll-bar-specific properties for the specified window.
 *
 * RETURNS
 *     nonzero if successful, or zero otherwise. If index is WSB_PROP_HSTYLE,
 *     the return is nonzero if InitializeFlatSB has been called for this window, or
 *     zero otherwise.
 */
BOOL WINAPI FlatSB_GetScrollProp(HWND hWnd, INT propIndex, LPINT prop)
{
    return FALSE;
}

/***********************************************************************
 *		FlatSB_SetScrollProp (COMCTL32.@)
 *
 * Sets flat-scroll-bar-specific properties for the specified window.
 *
 * RETURNS
 *     Success: Non-zero
 *     Failure: Zero
 */
BOOL WINAPI FlatSB_SetScrollProp(HWND hWnd, UINT index, INT newValue, BOOL flag)
{
    return FALSE;
}

/***********************************************************************
 *		FlatSB_EnableScrollBar (COMCTL32.@)
 *
 * See EnableScrollBar.
 */
BOOL WINAPI FlatSB_EnableScrollBar(HWND hWnd, int nBar, UINT flags)
{
    return EnableScrollBar(hWnd, nBar, flags);
}

/***********************************************************************
 *		FlatSB_ShowScrollBar (COMCTL32.@)
 *
 * See ShowScrollBar.
 */
BOOL WINAPI FlatSB_ShowScrollBar(HWND hWnd, int nBar, BOOL fShow)
{
    return ShowScrollBar(hWnd, nBar, fShow);
}

/***********************************************************************
 *		FlatSB_GetScrollRange (COMCTL32.@)
 *
 * See GetScrollRange.
 */
BOOL WINAPI FlatSB_GetScrollRange(HWND hWnd, int nBar, LPINT min, LPINT max)
{
    return GetScrollRange(hWnd, nBar, min, max);
}

/***********************************************************************
 *		FlatSB_GetScrollInfo (COMCTL32.@)
 *
 * See GetScrollInfo.
 */
BOOL WINAPI FlatSB_GetScrollInfo(HWND hWnd, int nBar, LPSCROLLINFO info)
{
    return GetScrollInfo(hWnd, nBar, info);
}

/***********************************************************************
 *		FlatSB_GetScrollPos (COMCTL32.@)
 *
 * See GetScrollPos.
 */
INT WINAPI FlatSB_GetScrollPos(HWND hWnd, int nBar)
{
    return GetScrollPos(hWnd, nBar);
}

/***********************************************************************
 *		FlatSB_SetScrollPos (COMCTL32.@)
 *
 * See SetScrollPos.
 */
INT WINAPI FlatSB_SetScrollPos(HWND hWnd, int nBar, INT pos, BOOL bRedraw)
{
    return SetScrollPos(hWnd, nBar, pos, bRedraw);
}

/***********************************************************************
 *		FlatSB_SetScrollInfo (COMCTL32.@)
 *
 * See SetScrollInfo.
 */
INT WINAPI FlatSB_SetScrollInfo(HWND hWnd, int nBar, LPSCROLLINFO info, BOOL bRedraw)
{
    return SetScrollInfo(hWnd, nBar, info, bRedraw);
}

/***********************************************************************
 *		FlatSB_SetScrollRange (COMCTL32.@)
 *
 * See SetScrollRange.
 */
INT WINAPI FlatSB_SetScrollRange(HWND hWnd, int nBar, INT min, INT max, BOOL bRedraw)
{
    return SetScrollRange(hWnd, nBar, min, max, bRedraw);
}

static LRESULT FlatSB_Create(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

static LRESULT FlatSB_Destroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

static LRESULT WINAPI FlatSB_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (!GetWindowLongPtrW(hWnd, 0) && (uMsg != WM_CREATE))
	    return DefWindowProcW(hWnd, uMsg, wParam, lParam);

    switch (uMsg)
    {
        case WM_CREATE:
            return FlatSB_Create(hWnd, wParam, lParam);

        case WM_DESTROY:
            return FlatSB_Destroy(hWnd, wParam, lParam);

        default:
            return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}

void FLATSB_Register()
{
    WNDCLASSW wndClass = {0};

    wndClass.style = CS_GLOBALCLASS;
    wndClass.lpfnWndProc = FlatSB_WindowProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = sizeof(DWORD*);
    wndClass.hCursor = LoadCursorW (0, (LPWSTR)IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndClass.lpszClassName = ClassName;

    RegisterClassW(&wndClass);
}

void FLATSB_Unregister()
{
    UnregisterClassW(ClassName, NULL);
}