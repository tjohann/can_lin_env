#
# build can_send.c
#

CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 `pkg-config --cflags libcap-ng`
LDLIBS = -I../libcan -I../libhelper -L../libhelper -L../libcan -lhelper -lcan -lpthread -lrt `pkg-config --libs libcap-ng`

PROG = can_send
OBJ = can_send.o

%.o: %.c
	$(CC) -g -c -o $@ $< $(CFLAGS) $(LDLIBS)

$(PROG): $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LDLIBS)

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

run_raw: $(PROG)
	./can_send -i vcan0 -a 0x123 -t 100 -p raw

run_bcm: $(PROG)
	./can_send -i vcan0 -a 0x456 -t 100 -p bcm

run_valgrind_raw:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./can_send -i vcan0 -a 0x123 -t 100 -p raw

run_valgrind_bcm:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./can_send -i vcan0 -a 0x456 -t 100 -p bcm

setup_vcan:
	(../../scripts/vcan_init.sh)

delete_vcan:
	(../../scripts/vcan_delete.sh)

rmmod_vcan:
	(../../scripts/vcan_rmmod.sh)

.PHONY: clean install uninstall rmmod_vcan delete_vcan setup_vcan
