#
# build lin_recv
#

CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11
LIBS = -I../liblin -I../libhelper -L../libhelper -lhelper -L../liblin -llin -lpthread -lrt

PROG = lin_recv
OBJ = lin_recv.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

$(PROG): $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

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
