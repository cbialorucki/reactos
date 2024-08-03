/*
 * COPYRIGHT:       GNU GPL, see /docs/licenses/COPYING
 * PROJECT:         ReactOS crt library
 * FILE:            lib/sdk/crt/printf/vswprintf.c
 * PURPOSE:         Implementation of vswprintf
 * PROGRAMMER:      Timo Kreuzer
 */

#define _sxprintf vswprintf
#define USE_COUNT 0
#define USE_VARARGS 1
#define _UNICODE

#include "_sxprintf.c"
