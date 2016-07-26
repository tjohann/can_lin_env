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


#include "libcan.h"

#define LIBCAN_EXPORT __attribute__ ((visibility ("default")))

static int
can_socket(char *ifname, struct sockaddr_can **addr, int proto, unsigned char flags)
{
	struct sockaddr_can *s = NULL;
	int fd_s = -1;

	if (proto == CAN_RAW)
		fd_s = socket(PF_CAN, SOCK_RAW, proto);
	if (proto == CAN_BCM)
		fd_s = socket(PF_CAN, SOCK_DGRAM, proto);
	if (fd_s == -1)
		goto error;

	struct ifreq ifr;
	memset(&ifr, 0, sizeof(struct ifreq));

	strncpy(ifr.ifr_name, ifname, strlen(ifname) + 1);
	int err = ioctl(fd_s, SIOCGIFINDEX, &ifr);
	if (err == -1)
		goto error;

	s = malloc(sizeof(struct sockaddr_can));
	if (s == NULL)
		goto error;

	memset(s, 0, sizeof(struct sockaddr_can));
	s->can_family  = AF_CAN;
	if (flags & BIND_TO_ANY_CANIF)
		s->can_ifindex = 0;
	else
		s->can_ifindex = ifr.ifr_ifindex;

	err = -1;
	if (proto == CAN_RAW)
		err = bind(fd_s, (struct sockaddr *) s,
			   sizeof(struct sockaddr_can));
	if (proto == CAN_BCM)
		err = connect(fd_s, (struct sockaddr *) s,
			      sizeof(struct sockaddr_can));
	if (err == -1)
		goto error;

	if (*addr != NULL)
		*addr = s;

	return fd_s;
error:
	perror("init_can_raw_socket()");
	if (s)
		free(s);

	return -1;
}

LIBCAN_EXPORT int
can_raw_socket(char *ifname, struct sockaddr_can **addr, unsigned char flags)
{
	return can_socket(ifname, addr, CAN_RAW, flags);
}

LIBCAN_EXPORT int
can_bcm_socket(char *ifname, struct sockaddr_can **addr, unsigned char flags)
{
	return can_socket(ifname, addr, CAN_BCM, flags);
}

LIBCAN_EXPORT int
set_hw_error_mask(int fd_s)
{
	/* see include/uapi/linux/can/error.h for all error codes */
	can_err_mask_t err_mask = (CAN_ERR_TX_TIMEOUT |
				   CAN_ERR_LOSTARB |
				   CAN_ERR_CRTL |
				   CAN_ERR_PROT |
				   CAN_ERR_TRX |
				   CAN_ERR_ACK |
				   CAN_ERR_BUSOFF |
				   CAN_ERR_BUSERROR |
				   CAN_ERR_RESTARTED
		);

	if (setsockopt(fd_s, SOL_CAN_RAW, CAN_RAW_ERR_FILTER,
		       &err_mask, sizeof(err_mask)) == -1) {
		perror("setsockopt");
		return -1;
	}

    return 0;
}
