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

#include "libhelper.h"

#define LIBHELPER_EXPORT __attribute__ ((visibility ("default")))

LIBHELPER_EXPORT int
drop_capability(int hold_cap)
{
	capng_clear(CAPNG_SELECT_BOTH);

	if (capng_update(CAPNG_ADD, CAPNG_EFFECTIVE | CAPNG_PERMITTED,
			 hold_cap) != 0) {
		printf("could not set capability -> %s\n",
		       capng_print_caps_text(CAPNG_PRINT_STDOUT,
					     CAPNG_EFFECTIVE));
		return -1;
	}

	capng_apply(CAPNG_SELECT_BOTH);

	return 0;
}

LIBHELPER_EXPORT char *
strlwr(char* str)
{
    char* tmp = str;

    for (;*tmp;++tmp)
        *tmp = tolower((unsigned char) *tmp);

    return str;
}

LIBHELPER_EXPORT void
ts_norm(struct timespec *t)
{
	while (t->tv_nsec >= NSEC_PER_SEC) {
		t->tv_nsec -= NSEC_PER_SEC;
		t->tv_sec++;
	}
}
