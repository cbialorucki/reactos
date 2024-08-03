/*
 * COPYRIGHT:   See /docs/licenses/COPYING
 * PROJECT:     ReactOS system libraries
 * FILE:        lib/sdk/crt/float/scalb.c
 * PURPOSE:     Floating-point number scaling
 * PROGRAMER:   Pierre Schweitzer (pierre@reactos.org)
 */

#include <precomp.h>

/*
 * @implemented
 */
double _scalb(double x, long exp)
{
	return ldexp(x, exp);
}
