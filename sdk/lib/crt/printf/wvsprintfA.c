/*
 * COPYRIGHT:       GNU GPL, see /docs/licenses/COPYING
 * PROJECT:         ReactOS crt library
 * FILE:            lib/sdk/crt/printf/wvsprintfA.c
 * PURPOSE:         Implementation of wvsprintfA
 * PROGRAMMER:      Timo Kreuzer
 */

#define _sxprintf wvsprintfA
#define USE_COUNT 0
#define USE_VARARGS 1
#define USER32_WSPRINTF

#include "_sxprintf.c"
