/*
 * COPYRIGHT:   See /docs/licenses/COPYING
 * PROJECT:     ReactOS TCP/IP protocol driver
 * FILE:        include/pool.h
 * PURPOSE:     Prototypes for memory pooling
 */

#pragma once

NDIS_STATUS PrependPacket( PNDIS_PACKET Packet, PCHAR Data, UINT Len,
			   BOOLEAN Copy );

/* EOF */
