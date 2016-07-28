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

#define PROGRAM_NAME "can_send"
static const char *program_name;

static void
__attribute__((noreturn)) usage(void)
{
	fprintf(stdout, "Usage: ./can_send -[haipt]                 \n");
	fprintf(stdout, "       -i vcan0     -> interface           \n");
	fprintf(stdout, "       -a 0x123     -> can addr            \n");
	fprintf(stdout, "       -t 100       -> cylic time 100ms    \n");
	fprintf(stdout, "       -p [raw/bcm] -> can socket protocol \n");
	fprintf(stdout, "       -h           -> show this help      \n");
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
	setsockopt(ifname_s, SOL_CAN_RAW, CAN_RAW_FILTER,
		   &rfilter, sizeof(rfilter));

	/* set all hardware related error mask bits */
	if (set_hw_error_mask(ifname_s) == -1) {
		eprintf("ERROR: could not set error mask\n");
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

		if (ioctl(ifname_s, SIOCGSTAMP, &tv) == -1)
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
send_cyclic_telegram(char *ifname, unsigned long can_addr,
		     unsigned long cycle_time_ms)
{
	const unsigned char default_ms = 100;

	int fds = can_raw_socket(ifname, NULL, 0);
	if (fds == -1) {
		eprintf("ERROR -> could not init can_node %s for CAN_RAW\n",
			ifname);
		exit(EXIT_FAILURE);
	}

	struct can_frame frame;
	memset(&frame, 0, sizeof(frame));

	frame.can_id  = can_addr;
	frame.can_dlc = 2;

	struct timespec t;
	memset(&t, 0, sizeof(struct timespec));

	t.tv_sec = 0;
	if (cycle_time_ms == 0) {
		printf("no cyclic time defined, use default value:%dms\n",
		       default_ms);
		t.tv_nsec = default_ms * 1000000;
	} else {
		t.tv_nsec = cycle_time_ms * 1000000;
		ts_norm(&t);
	}

	ssize_t send_nbytes = -1;
	for (;;) {
		send_nbytes = write(fds, &frame, sizeof(struct can_frame));
		if (send_nbytes == -1) {
			perror("sendto");
			/* stop sending for some time -> user recognize it */
			sleep(4);
			continue;
		}

		if (frame.data[1] == 0xfe)
			frame.data[1] = 0;
		else
			frame.data[1] += 1;

		clock_nanosleep(CLOCK_MONOTONIC, 0, &t, NULL);
	}

	/* should never reached */
	close(fds);
}

void
send_cyclic_telegram_via_bcm(char *ifname, unsigned long can_addr,
			     unsigned long cycle_time_ms)
{
	int fds = can_bcm_socket(ifname, NULL, 0);
	if (fds == -1) {
		eprintf("ERROR -> could not init can_node %s for CAN_BCM\n",
			ifname);
		exit(EXIT_FAILURE);
	}

	sleep(20);

	printf("Not yet implemented\n");
	close(fds);

	exit(EXIT_SUCCESS);
}

int
main(int argc, char *argv[])
{
	unsigned long can_addr = 0;
	unsigned long cycle_time_ms = 0;
	char *ifname = NULL;
	char *can_proto = NULL;

	int c;
	while ((c = getopt(argc, argv, "hei:a:p:t:")) != -1) {
		switch (c) {
		case 'i':
			ifname = optarg;
			break;
		case 'p':
			can_proto = optarg;
			break;
		case 'a':
			can_addr = strtoul(optarg, NULL, 16);
			break;
		case 't':
			cycle_time_ms = strtoul(optarg, NULL, 10);
			break;
		case 'h':
			usage();
			break;
		default:
			eprintf("ERROR: no valid argument\n");
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

	if (can_addr == 0)
		usage();
	else
		printf("will use addr 0x%x\n", (int) can_addr);

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
		send_cyclic_telegram(ifname, can_addr, cycle_time_ms);

	if (strncmp(can_proto, "bcm", 3) == 0)
		send_cyclic_telegram_via_bcm(ifname, can_addr, cycle_time_ms);

	/* should never reached */
	eprintf("ERROR -> something went wrong, you should never reach this\n");

	(void) pthread_join(tid, NULL);

	exit(EXIT_SUCCESS);
}
