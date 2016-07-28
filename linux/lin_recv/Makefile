#
# build lin_recv
#

CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 `pkg-config --cflags libcap-ng`
LDLIBS = -I../liblin -I../libhelper -L../libhelper -L../liblin -lhelper -llin -lpthread -lrt `pkg-config --libs libcap-ng`

PROG = lin_recv
OBJ = lin_recv.o

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

.PHONY: clean install uninstall
