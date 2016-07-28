#
# build liblin.a
#

CC = gcc
CFLAGS = -Wall -std=gnu11 `pkg-config --cflags libcap-ng`
#LDLIBS = -I../libhelper -L../libhelper -lhelper -lpthread -lrt `pkg-config --libs libcap-ng`
LDLIBS = -I../libhelper

LIB = liblin.a
OBJ = liblin.o

%.o: %.c
	$(CC) -g -c -o $@ $< $(CFLAGS) $(LDLIBS)

$(LIB): $(OBJ)
	ar rcs $@ $<

install: $(LIB)
	install $(LIB) $(HOME)/lib/$(LIB)

uninstall:
	rm -rf $(HOME)/lib/$(LIB)

clean:
	rm -rf *~
	rm -rf .*~
	rm -rf *.o
	rm -rf $(LIB)

distclean: clean

.PHONY: clean install uninstall
