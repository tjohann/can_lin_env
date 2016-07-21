#
# build can_send.c
#

CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11
LIBS = -I../libcan -I../libhelper -L../libhelper -lhelper -L../libcan -lcan -lpthread -lrt 

PROG = can_send
OBJ = can_send.o

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
