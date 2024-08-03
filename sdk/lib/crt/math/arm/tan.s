/*
 * COPYRIGHT:         BSD - See /docs/licenses/COPYING.ARM
 * PROJECT:           ReactOS CRT library
 * PURPOSE:           Implementation of tan
 * PROGRAMMER:        Timo Kreuzer (timo.kreuzer@reactos.org)
 */

/* INCLUDES ******************************************************************/

#include <kxarm.h>

/* CODE **********************************************************************/

    TEXTAREA

    LEAF_ENTRY tan

	__assertfail
	bx	lr

    LEAF_END tan

    END
/* EOF */
