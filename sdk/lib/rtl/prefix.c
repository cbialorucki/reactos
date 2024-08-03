/*
 * PROJECT:         ReactOS Run-Time Library
 * LICENSE:         BSD - See /docs/licenses/COPYING.ARM
 * PURPOSE:         Rtl Prefix Routines
 */

/* INCLUDES *******************************************************************/

#include <rtl.h>
#define NDEBUG
#include <debug.h>

/* FUNCTIONS ******************************************************************/

VOID
NTAPI
PfxInitialize(IN PPREFIX_TABLE PrefixTable)
{
    UNIMPLEMENTED;
}

BOOLEAN
NTAPI
PfxInsertPrefix(IN PPREFIX_TABLE PrefixTable,
                IN PSTRING Prefix,
                IN PPREFIX_TABLE_ENTRY PrefixTableEntry)
{
    UNIMPLEMENTED;
    return FALSE;
}

VOID
NTAPI
PfxRemovePrefix(IN PPREFIX_TABLE PrefixTable,
                IN PPREFIX_TABLE_ENTRY PrefixTableEntry)
{
    UNIMPLEMENTED;
}

PPREFIX_TABLE_ENTRY
NTAPI
PfxFindPrefix(IN PPREFIX_TABLE PrefixTable,
              IN PSTRING FullName)
{
    UNIMPLEMENTED;
    return FALSE;
}
