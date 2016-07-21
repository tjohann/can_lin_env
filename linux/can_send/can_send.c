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
	fprintf(stdout, "Usage: ./can_send -[hai]      \n");
	fprintf(stdout, "       -i vcan0 -> interface  \n");
	fprintf(stdout, "       -a 0x123 -> can addr   \n");
	fprintf(stdout, "       -h -> show this help   \n");
	putchar('\n');

	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	unsigned long can_addr = 0;
	char *ifname = NULL;

	int c;
	while ((c = getopt(argc, argv, "hei:a:")) != -1) {
		switch (c) {
		case 'i':
			ifname = optarg;
			break;
		case 'a':
			can_addr = strtoul(optarg, NULL, 16);
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
		printf("Will use %s as canif\n", ifname);

	if (can_addr == 0)
		usage();
	else
		printf("Will use addr 0x%x\n", (int) can_addr);

	program_name = PROGRAM_NAME;

	can_node_t cnode;
	if (can_raw_server(&cnode, ifname, can_addr) == -1) {
		fprintf(stderr, "ERROR -> could not init can_node %s\n", ifname);
		exit(EXIT_FAILURE);
	}

	sleep(20);

	return EXIT_SUCCESS;
}
