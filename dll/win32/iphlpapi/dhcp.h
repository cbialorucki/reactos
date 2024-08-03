/*
 * PROJECT:     ReactOS Networking
 * LICENSE:     GPL - See /docs/licenses/COPYING
 * FILE:        lib/iphlpapi/dhcp_reactos.c
 * PURPOSE:     DHCP helper functions for ReactOS
 * COPYRIGHT:   Copyright 2006 Ge van Geldorp <gvg@reactos.org>
 */

#ifndef WINE_DHCP_H_
#define WINE_DHCP_H_

DWORD getDhcpInfoForAdapter(DWORD AdapterIndex,
                            PBOOL DhcpEnabled,
                            PDWORD DhcpServer,
                            time_t *LeaseObtained,
                            time_t *LeaseExpires);

#endif /* ndef WINE_DHCP_H_ */
