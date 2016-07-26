#
# build lin_send.c
#

CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 `pkg-config --cflags libcap-ng`
LDLIBS = -I../liblin -I../libhelper -L../libhelper -L../liblin -lhelper -llin -lpthread -lrt `pkg-config --libs libcap-ng`

PROG = lin_send
OBJ = lin_send.o

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

.PHONY: clean install uninstall
