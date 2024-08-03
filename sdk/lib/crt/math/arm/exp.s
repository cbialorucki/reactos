/*
 * COPYRIGHT:         BSD - See /docs/licenses/COPYING.ARM
 * PROJECT:           ReactOS CRT library
 * PURPOSE:           Implementation of exp
 * PROGRAMMER:        Timo Kreuzer (timo.kreuzer@reactos.org)
 */

/* INCLUDES ******************************************************************/

#include <kxarm.h>

/* CODE **********************************************************************/

    TEXTAREA

    LEAF_ENTRY exp

	__assertfail
	bx	lr

    LEAF_END exp

    END
/* EOF */
