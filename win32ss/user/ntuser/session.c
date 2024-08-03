/*
 * COPYRIGHT:        See /docs/licenses/COPYING
 * PROJECT:          ReactOS kernel
 * PURPOSE:          Session stuff
 * FILE:             win32ss/user/ntuser/session.c
 * PROGRAMER:        Gunnar
 */

#include <win32k.h>

/* GLOBALS *******************************************************************/

PRTL_ATOM_TABLE gAtomTable = NULL;

/* FUNCTIONS *******************************************************************/

NTSTATUS FASTCALL
InitSessionImpl(VOID)
{
   return RtlCreateAtomTable(37, &gAtomTable);
}

