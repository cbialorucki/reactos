/*
 * COPYRIGHT:       See /docs/licenses/COPYING
 * PROJECT:         FATX File System Management
 * FILE:            dll/win32/ufatx/ufatx.c
 * PURPOSE:         ufatx DLL initialisation
 * PROGRAMMERS:     Herv� Poussineau (hpoussin@reactos.org)
 */

#include <windef.h>

INT WINAPI
DllMain(
    IN HINSTANCE hinstDLL,
    IN DWORD     dwReason,
    IN LPVOID    lpvReserved)
{
    UNREFERENCED_PARAMETER(hinstDLL);
    UNREFERENCED_PARAMETER(dwReason);
    UNREFERENCED_PARAMETER(lpvReserved);

    return TRUE;
}
