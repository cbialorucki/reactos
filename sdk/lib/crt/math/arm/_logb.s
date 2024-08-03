/*
 * COPYRIGHT:         BSD - See /docs/licenses/COPYING.ARM
 * PROJECT:           ReactOS CRT library
 * PURPOSE:           Implementation of _logb
 * PROGRAMMER:        Timo Kreuzer (timo.kreuzer@reactos.org)
 */

/* INCLUDES ******************************************************************/

#include <kxarm.h>

/* CODE **********************************************************************/

    TEXTAREA

    LEAF_ENTRY _logb

	__assertfail
	bx	lr

    LEAF_END _logb

    END
/* EOF */
