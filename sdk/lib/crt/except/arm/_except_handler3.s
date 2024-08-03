/*
 * COPYRIGHT:         BSD - See /docs/licenses/COPYING.ARM
 * PROJECT:           ReactOS CRT librariy
 * PURPOSE:           Implementation of _except_handler3
 * PROGRAMMER:        Timo Kreuzer (timo.kreuzer@reactos.org)
 */

/* INCLUDES ******************************************************************/

#include <kxarm.h>

/* CODE **********************************************************************/
    TEXTAREA

    LEAF_ENTRY _except_handler3
    __assertfail
    bx lr
    LEAF_END _except_handler3

    END
/* EOF */
