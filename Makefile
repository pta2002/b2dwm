CFLAGS += -std=c99 -Wall -Wextra -pedantic -Wold-style-declaration
CFLAGS += -Wmissing-prototypes -Wno-unused-parameter
PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin
CC     ?= gcc
CXX	   ?= g++

all: config.h b2dwm

config.h:
	cp config.def.h config.h

b2dwm: b2dwm.c b2dwm.h config.h physics.o
	$(CC) -O3 $(CFLAGS) -o b2dwm b2dwm.c physics.o -lX11 $(LDFLAGS) -lBox2D -lstdc++

physics.o: physics.cpp config.h
	$(CXX) physics.cpp -O3 $(CFLAGS) -c $(LDFLAGS)

install: all
	install -Dm755 b2dwm $(DESTDIR)$(BINDIR)/b2dwm

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/b2dwm

clean:
	rm -f b2dwm *.o
