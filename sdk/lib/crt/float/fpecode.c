/*
 * COPYRIGHT:   See /docs/licenses/COPYING
 * PROJECT:     ReactOS system libraries
 * FILE:        lib/sdk/crt/float/fpecode.c
 * PURPOSE:     Unknown
 * PROGRAMER:   Unknown
 * UPDATE HISTORY:
 *              25/11/05: Added license header
 */

#include <precomp.h>

/*
 * @implemented
 */
int * __fpecode(void)
{
    return &msvcrt_get_thread_data()->fpecode;
}
