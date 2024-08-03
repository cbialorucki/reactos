/*
 * COPYRIGHT:       GNU GPL, see /docs/licenses/COPYING
 * PROJECT:         ReactOS crt library
 * FILE:            lib/sdk/crt/printf/_vsnprintf.c
 * PURPOSE:         Implementation of _vsnprintf
 * PROGRAMMER:      Timo Kreuzer
 */

#define _sxprintf _vsnprintf
#define USE_COUNT 1
#define USE_VARARGS 1

#include "_sxprintf.c"
