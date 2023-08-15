/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     "Theming - Initialization" (figure out how to name this professionally)
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "comctl32.h"
#include "uxtheme.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(theming);

typedef LRESULT (CALLBACK* THEMING_SUBCLASSPROC)(HWND, UINT, WPARAM, LPARAM, ULONG_PTR);

extern LRESULT CALLBACK THEMING_ScrollbarSubclassProc (HWND, UINT, WPARAM, LPARAM, ULONG_PTR) DECLSPEC_HIDDEN;

static const struct ThemingSubclass
{
    const WCHAR* className;
    THEMING_SUBCLASSPROC subclassProc;
} subclasses[] = {
    /* Note: list must be sorted by class name */
    {WC_SCROLLBARW,        THEMING_ScrollbarSubclassProc}
};

#define NUM_SUBCLASSES        (ARRAY_SIZE(subclasses))

static WNDPROC originalProcs[NUM_SUBCLASSES];
static ATOM atRefDataProp;
static ATOM atSubclassProp;

/* Generate a number of subclass window procs.
 * With a single proc alone, we can't really reliably find out the superclass,
 * so have one for each subclass. The subclass number is also stored in a prop
 * since it's needed by THEMING_CallOriginalClass(). Then, the subclass
 * proc and ref data are fetched and the proc called.
 */
#define MAKE_SUBCLASS_PROC(N)                                               \
static LRESULT CALLBACK subclass_proc ## N (HWND wnd, UINT msg,             \
                                            WPARAM wParam, LPARAM lParam)   \
{                                                                           \
    LRESULT result;                                                         \
    ULONG_PTR refData;                                                      \
    SetPropW (wnd, (LPCWSTR)MAKEINTATOM(atSubclassProp), (HANDLE)N);        \
    refData = (ULONG_PTR)GetPropW (wnd, (LPCWSTR)MAKEINTATOM(atRefDataProp)); \
    TRACE ("%d; (%p, %x, %lx, %lx, %lx)\n", N, wnd, msg, wParam, lParam,     \
        refData);                                                           \
    result = subclasses[N].subclassProc (wnd, msg, wParam, lParam, refData);\
    TRACE ("result = %lx\n", result);                                       \
    return result;                                                          \
}

MAKE_SUBCLASS_PROC(0)

static const WNDPROC subclassProcs[NUM_SUBCLASSES] = {
    subclass_proc0,
};

/***********************************************************************
 * THEMING_Initialize
 *
 * Register classes for standard controls that will shadow the system
 * classes.
 */
void THEMING_Initialize(HANDLE hActCtx5, HANDLE hActCtx6)
{
    unsigned int i;
    static const WCHAR subclassPropName[] = L"CC32ThemingSubCl";
    static const WCHAR refDataPropName[] = L"CC32ThemingData";

    ULONG_PTR ulCookie;
    BOOL ret, bActivated;

    atSubclassProp = GlobalAddAtomW(subclassPropName);
    atRefDataProp = GlobalAddAtomW(refDataPropName);

    for (i = 0; i < NUM_SUBCLASSES; i++)
    {
        WNDCLASSEXW WindowClass;

        WindowClass.cbSize = sizeof(WindowClass);

        bActivated = ActivateActCtx(hActCtx5, &ulCookie);
        ret = GetClassInfoExW (NULL, subclasses[i].className, &WindowClass);
        if (bActivated)
            DeactivateActCtx(0, ulCookie);

        if (!ret)
        {
            ERR("Could not retrieve information for class %s\n",
                debugstr_w(subclasses[i].className));
            continue;
        }
        originalProcs[i] = WindowClass.lpfnWndProc;
        WindowClass.lpfnWndProc = subclassProcs[i];
        WindowClass.style |= CS_GLOBALCLASS;
        WindowClass.hInstance = COMCTL32_hModule;

        if (!WindowClass.lpfnWndProc)
        {
            ERR("Missing proc for class %s\n",
                debugstr_w (subclasses[i].className));
            continue;
        }

        bActivated = ActivateActCtx(hActCtx6, &ulCookie);
        if (!RegisterClassExW (&WindowClass))
        {
            WARN("Could not re-register class %s: %x\n",
                debugstr_w (subclasses[i].className), GetLastError ());
        }
        else
        {
            TRACE("Re-registered class %s\n",
                debugstr_w (subclasses[i].className));
        }

        if (bActivated)
            DeactivateActCtx(0, ulCookie);
    }
}

/***********************************************************************
 * THEMING_Uninitialize
 *
 * Unregister shadow classes for standard controls.
 */
void THEMING_Uninitialize (void)
{
    unsigned int i;

    if (!atSubclassProp) return;  /* not initialized */

    for (i = 0; i < NUM_SUBCLASSES; i++)
    {
        UnregisterClassW(subclasses[i].className, NULL);
    }
}

/***********************************************************************
 * THEMING_CallOriginalClass
 *
 * Determines the original window proc and calls it.
 */
LRESULT THEMING_CallOriginalClass (HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    INT_PTR subclass = (INT_PTR)GetPropW (wnd, (LPCWSTR)MAKEINTATOM(atSubclassProp));
    WNDPROC oldProc = originalProcs[subclass];
    return CallWindowProcW (oldProc, wnd, msg, wParam, lParam);
}

/***********************************************************************
 * THEMING_SetSubclassData
 *
 * Update the "refData" value of the subclassed window.
 */
void THEMING_SetSubclassData (HWND wnd, ULONG_PTR refData)
{
    SetPropW (wnd, (LPCWSTR)MAKEINTATOM(atRefDataProp), (HANDLE)refData);
}