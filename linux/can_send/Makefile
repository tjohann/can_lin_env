#
# build can_send.c
#

CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 `pkg-config --cflags libcap-ng`
LDLIBS = -I../libcan -I../libhelper -L../libhelper -L../libcan -lhelper -lcan -lpthread -lrt `pkg-config --libs libcap-ng`

PROG = can_send
OBJ = can_send.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDLIBS)

$(PROG): $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LDLIBS)

install: $(PROG)
	install $(PROG) $(HOME)/bin/$(PROG)

uninstall:
	rm -rf $(HOME)/bin/$(PROG)
clean:
	rm -rf *~
	rm -rf .*~
	rm -rf *.o
	rm -rf $(PROG)

distclean: clean

setup_vcan:
	(../../scripts/vcan_init.sh)

delete_vcan:
	(../../scripts/vcan_delete.sh)

rmmod_vcan:
	(../../scripts/vcan_rmmod.sh)

.PHONY: clean install uninstall rmmod_vcan delete_vcan setup_vcan
