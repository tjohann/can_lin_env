/*
  GPL
  (c) 2015-2016, thorsten.johannvorderbrueggen@t-online.de

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

#define PROGRAM_NAME "can_recv"
static const char *program_name;

static void
__attribute__((noreturn)) usage(void)
{
	fprintf(stdout, "Usage: ./can_recv -[hip]       \n");
	fprintf(stdout, "       -i vcan0 -> interface  \n");
	fprintf(stdout, "       -f 0x00F,0x10A -> recv 0x00F and 0x10A  \n");
	fprintf(stdout, "       -p [raw/bcm] -> can socket protocol \n");
	fprintf(stdout, "       -h -> show this help   \n");
	putchar('\n');

	exit(EXIT_FAILURE);
}

void *
print_error_frames(void *arg)
{
	char *ifname = (char *) arg;

	int ifname_s = can_raw_socket(ifname, NULL, 0);
	if (ifname_s == -1) {
		eprintf("ERROR -> could not init can_node %s\n", ifname);
		return NULL;
	}

	struct can_filter rfilter;
	rfilter.can_id   = 0x00;
	rfilter.can_mask = CAN_SFF_MASK;
	if (setsockopt(ifname_s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter,
		       sizeof(rfilter)) == -1) {
		eprintf("could not set can filter");
		return NULL;
	}

	/* set all hardware related error mask bits */
	if (set_hw_error_mask(ifname_s) == -1) {
		eprintf("ERROR -> could not set error mask\n");
		return NULL;
	}

	struct can_frame frame;
	struct timeval tv;
	ssize_t recv_nbytes = -1;
	for (;;) {
		memset(&frame, 0, sizeof(frame));
		memset(&tv, 0, sizeof(tv));

		recv_nbytes = read(ifname_s, &frame, sizeof(struct can_frame));
		if (recv_nbytes == -1) {
			perror("read in print_error_frames");
			continue;
		}

		if (ioctl(ifname_s, SIOCGSTAMP_OLD, &tv) == -1)
			perror("ioctl in print_error_frames");

		if (frame.can_id & CAN_ERR_FLAG) {
			printf("\n!-----! An ERROR flag is set !-----!\n");

			printf("%s: received can frame -> \"frame.id:0x%x\" / \
\"frame.can_dlc:%d\" @ \"time:%s",
			       __FUNCTION__, frame.can_id, frame.can_dlc,
			       ctime((const time_t *) &tv.tv_sec));

			for (int i = 0; i < frame.can_dlc; i++)
				printf("frame.data[%d] -> 0x%.2x\n", i, frame.data[i]);
		} else {
			eprintf("ERROR -> something went wrong ... \
should never see that!\n");
		}
	}

	/* should never reached */
	close(ifname_s);

	return NULL;
}

void
recv_frame_raw(char *ifname, char *flist)
{
	int fds = can_raw_socket(ifname, NULL, 0);
	if (fds == -1) {
		eprintf("ERROR -> could not init can_node %s for CAN_RAW\n",
			ifname);
		exit(EXIT_FAILURE);
	}

	if (set_flist(fds, flist) == -1)
		eprintf("could not set filter list\n");

	struct timeval tv;
	struct can_frame frame;
	ssize_t n = -1;
	for (;;) {
		memset(&frame, 0, sizeof(frame));
		memset(&tv, 0, sizeof(tv));

		n = read(fds, &frame, sizeof(struct can_frame));
		if (n == -1) {
			perror("read in recv_frame_raw");
			continue;
		} else {
			if (n < (int) sizeof(struct can_frame))
				eprintf("incomplete can frame in recv_frame_raw\n");

			printf("received %d bytes\n", (int) n);
		}

		ioctl(fds, SIOCGSTAMP_OLD, &tv);

		/* TODO: check if this really occures due to error handle thread */
		if (frame.can_id & CAN_ERR_FLAG)
			printf("!-----! An ERROR flag is set !-----!\n");

		printf("\n%s: received can frame -> \"frame.id:0x%x\" / \
\"frame.can_dlc:%d\" @ \"time:%s",
		       __FUNCTION__, frame.can_id, frame.can_dlc,
		       ctime((const time_t *) &tv.tv_sec));

		for (int i = 0; i < frame.can_dlc; i++)
			printf("frame.data[%d] -> 0x%.2x\n", i, frame.data[i]);
	}

	/* should never reached */
	close(fds);
}

void
recv_frame_bcm(char *ifname, char *flist)
{
	int fds = can_bcm_socket(ifname, NULL, 0);
	if (fds == -1) {
		eprintf("ERROR -> could not init can_node %s for CAN_RAW\n",
			ifname);
		exit(EXIT_FAILURE);
	}

	sleep(10);

	/* should never reached */
	close(fds);
}

int
main(int argc, char *argv[])
{
	char *ifname = NULL;
	char *can_proto = NULL;
	char *flist = NULL;

	int c;
	while ((c = getopt(argc, argv, "hi:p:f:")) != -1) {
		switch (c) {
		case 'i':
			ifname = optarg;
			break;
		case 'p':
			can_proto = optarg;
			break;
		case 'f':
			flist = optarg;
			break;
		case 'h':
			usage();
			break;
		default:
			eprintf("ERROR -> no valid argument\n");
			usage();
		}
	}

	if (ifname == NULL)
		usage();
	else
		printf("will use %s as canif\n", ifname);

	if (can_proto == NULL)
		usage();
	else
		printf("will use can_%s as can protocol\n", can_proto);

	if (flist == NULL)
		printf("no can-id filter will be set\n");

	program_name = PROGRAM_NAME;
	strlwr(can_proto);

	/* thread to receive hardware related error frames -> print the frame */
	pthread_t tid;
	int ret = pthread_create(&tid, NULL, print_error_frames, ifname);
	if (ret != 0) {
		errno = ret;
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	if (strncmp(can_proto, "raw", 3) == 0)
		recv_frame_raw(ifname, flist);

	if (strncmp(can_proto, "bcm", 3) == 0)
		recv_frame_bcm(ifname, flist);

	/* should never reached */
	eprintf("ERROR -> something went wrong, you should never reach this\n");

	(void) pthread_join(tid, NULL);

	exit(EXIT_SUCCESS);
}
