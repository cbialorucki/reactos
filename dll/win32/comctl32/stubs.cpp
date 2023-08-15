/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Common control stubs
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#include <windef.h>
#include <reactos/debug.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef PVOID PREADERMODEINFO;

HWND WINAPI CreatePage(DWORD dwInitParam, HWND hWndParent)
{
    UNIMPLEMENTED;
    return NULL;
}

HWND WINAPI CreateProxyPage(DWORD dwUnknown1, DWORD dwUnknown2)
{
    UNIMPLEMENTED;
    return NULL;
}

VOID WINAPI DoReaderMode(PREADERMODEINFO prmi)
{
    UNIMPLEMENTED;
}

VOID WINAPI SHGetProcessDword(DWORD dwUnknown1, DWORD dwUnknown2)
{
    UNIMPLEMENTED;
}
#ifdef __cplusplus
}
#endif