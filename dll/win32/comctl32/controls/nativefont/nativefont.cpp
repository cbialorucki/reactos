/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Native font control
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
/* NOTES
 *   This is just a dummy control. An author is needed! Any volunteers?
 *   I will only improve this control once in a while.
 *     Eric <ekohl@abo.rhein-zeitung.de>
 *
 * TODO:
 *   - All messages.
 *   - All notifications.
 */

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "commctrl.h"
#include "comctl32.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(nativefont);

typedef struct
{
    HWND m_hwnd;
} NATIVEFONT_INFO;

#define NATIVEFONT_GetInfoPtr(hwnd) ((NATIVEFONT_INFO *)GetWindowLongPtrW (hwnd, 0))

static LRESULT NATIVEFONT_Create (HWND hwnd)
{
    NATIVEFONT_INFO *infoPtr;

    /* allocate memory for info structure */
    infoPtr = (NATIVEFONT_INFO*)Alloc(sizeof(NATIVEFONT_INFO));
    SetWindowLongPtrW (hwnd, 0, (DWORD_PTR)infoPtr);

    /* initialize info structure */
    infoPtr->m_hwnd = hwnd;

    return 0;
}

static LRESULT NATIVEFONT_Destroy (NATIVEFONT_INFO *infoPtr)
{
    /* free control info data */
    SetWindowLongPtrW(infoPtr->m_hwnd, 0, 0);
    Free(infoPtr);

    return 0;
}

static LRESULT WINAPI NATIVEFONT_WindowProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    NATIVEFONT_INFO *infoPtr = NATIVEFONT_GetInfoPtr(hwnd);

    TRACE("hwnd=%p msg=%04x wparam=%08lx lparam=%08lx\n",
	  hwnd, uMsg, wParam, lParam);

    if (!infoPtr && (uMsg != WM_CREATE))
	return DefWindowProcW( hwnd, uMsg, wParam, lParam );

    switch (uMsg)
    {
	case WM_CREATE:
	    return NATIVEFONT_Create (hwnd);

	case WM_DESTROY:
	    return NATIVEFONT_Destroy (infoPtr);

        case WM_MOVE:
        case WM_SIZE:
        case WM_SHOWWINDOW:
        case WM_WINDOWPOSCHANGING:
        case WM_WINDOWPOSCHANGED:
        case WM_SETFONT:
        case WM_GETDLGCODE:
	    /* FIXME("message %04x seen but stubbed\n", uMsg); */
	    return DefWindowProcW (hwnd, uMsg, wParam, lParam);

	default:
	    if ((uMsg >= WM_USER) && (uMsg < WM_APP) && !COMCTL32_IsReflectedMessage(uMsg))
		ERR("unknown msg %04x wp=%08lx lp=%08lx\n",
		     uMsg, wParam, lParam);
	    return DefWindowProcW (hwnd, uMsg, wParam, lParam);
    }
}


VOID NATIVEFONT_Register (void)
{
    WNDCLASSW wndClass;

    ZeroMemory (&wndClass, sizeof(WNDCLASSW));
    wndClass.style         = CS_GLOBALCLASS;
    wndClass.lpfnWndProc   = NATIVEFONT_WindowProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = sizeof(NATIVEFONT_INFO *);
    wndClass.hCursor       = LoadCursorW (0, (LPWSTR)IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wndClass.lpszClassName = WC_NATIVEFONTCTLW;

    RegisterClassW (&wndClass);
}


VOID NATIVEFONT_Unregister (void)
{
    UnregisterClassW (WC_NATIVEFONTCTLW, NULL);
}
