/*
 * PROJECT:         ReactOS DiskPart
 * LICENSE:         GPL - See /docs/licenses/COPYING
 * FILE:            base/system/diskpart/automount.c
 * PURPOSE:         Manages all the partitions of the OS in an interactive way.
 * PROGRAMMERS:     Lee Schroeder
 */

#include "diskpart.h"

BOOL automount_main(INT argc, LPWSTR *argv)
{
    ConPuts(StdOut, L"Automount\n");
    return TRUE;
}
