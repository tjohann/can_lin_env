/*
  GPL
  (c) 2016, thorsten.johannvorderbrueggen@t-online.de

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

#ifndef __LIBCAN_H__
#define __LIBCAN_H__

#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>

#include "libhelper.h"

/* can_*_socket flags */
#define BIND_TO_SINGLE_CANIF 0x00
#define BIND_TO_ANY_CANIF 0x01

/* get a can socket -> CAN_RAW */
int
can_raw_socket(char *ifname, struct sockaddr_can **addr, unsigned char flags);
/* get a can socket -> CAN_BCM */
int
can_bcm_socket(char *ifname, struct sockaddr_can **addr, unsigned char flags);

/* set all hardware related error mask bits */
int
set_hw_error_mask(int fd_s);

#endif
