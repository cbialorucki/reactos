/*
 * COPYRIGHT:         BSD - See /docs/licenses/COPYING.ARM
 * PROJECT:           ReactOS CRT librariy
 * PURPOSE:           Implementation of _global_unwind2
 * PROGRAMMER:        Timo Kreuzer (timo.kreuzer@reactos.org)
 */

/* INCLUDES ******************************************************************/

#include <kxarm.h>

/* CODE **********************************************************************/
    TEXTAREA

    LEAF_ENTRY _global_unwind2
    __assertfail
    bx lr
    LEAF_END _global_unwind2

    END
/* EOF */
