/*

Copyright (C) 2012   Michael Dirska, DL1BFF (dl1bff@mdx.de)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/*
 * lldp.c
 *
 * Created: 10.05.2012 12:50:11
 *  Author: mdirska
 */ 


/* 
	LLDP frames
	
	IEEE 802.1AB-2009
	
*/

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "gcc_builtin.h"

#include "up_io/eth_txmem.h"
#include "up_io/eth.h"

#include "lldp.h"


const uint8_t lldp_frame[] =
{
	0x01, 0x80, 0xC2, 0x00, 0x00, 0x0E,  // dest  "nearest bridge" 01-80-C2-00-00-0E 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // src
	0x00, 0x00, // type
	
	0x02, 0x07, // chassis ID, ethernet MAC
	  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	
	0x04, 0x07, // port ID, ethernet MAC
	  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	
	0x06, 0x02, 0x00, 0x0A,  // TTL 10 seconds
	
	0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // system description
	
	0x00, 0x00  // END
};


void lldp_send (void)
{
	eth_txmem_t * packet = eth_txmem_get( sizeof lldp_frame );
	
	if (packet == NULL) // nomem
		return;
		
	memcpy(packet->data, lldp_frame, sizeof lldp_frame);
	
	memcpy(packet->data + 17, mac_addr, 6); // 6 byte mac address
	memcpy(packet->data + 26, mac_addr, 6); // 6 byte mac address
	
	memcpy(packet->data + 38, "UP4DAR", 6);
	
	eth_set_src_mac_and_type(packet->data, 0x88cc);
	
	eth_txmem_send(packet);
}