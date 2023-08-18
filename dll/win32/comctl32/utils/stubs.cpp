/*
 * PROJECT:     ReactOS common controls
 * LICENSE:     LGPL-2.1-or-later (https://spdx.org/licenses/LGPL-2.1-or-later)
 * PURPOSE:     Common control stubs
 * COPYRIGHT:   Copyright 2023 Carl Bialorucki <cbialo2@outlook.com>
 */
#include <windef.h>
#include <reactos/debug.h>

typedef PVOID PREADERMODEINFO;

extern "C" HWND WINAPI CreatePage(DWORD dwInitParam, HWND hWndParent)
{
    UNIMPLEMENTED;
    return NULL;
}

extern "C" HWND WINAPI CreateProxyPage(DWORD dwUnknown1, DWORD dwUnknown2)
{
    UNIMPLEMENTED;
    return NULL;
}

extern "C" void WINAPI DoReaderMode(PREADERMODEINFO prmi)
{
    UNIMPLEMENTED;
}

extern "C" void WINAPI SHGetProcessDword(DWORD dwUnknown1, DWORD dwUnknown2)
{
    UNIMPLEMENTED;
}