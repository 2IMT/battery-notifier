CC ?= gcc
CFLAGS = -std=c17 -Wall -Wextra
LDFLAGS =
MYCFLAGS =

INSTALL_PREFIX = /usr/local

all: battery-notifier

battery-notifier: main.o envargs.o
	$(CC) $(CFLAGS) $(MYCFLAGS) -o battery-notifier main.o envargs.o

main.o: main.c envargs.h
	$(CC) $(CFLAGS) $(MYCFLAGS) $(LDFLAGS) -c main.c

envargs.o: envargs.c envargs.h
	$(CC) $(CFLAGS) $(MYCFLAGS) $(LDFLAGS) -c envargs.c

.PHONY: install
install: battery-notifier
	install -Dm755 battery-notifier $(INSTALL_PREFIX)/bin/battery-notifier

.PHONY: uninstall
uninstall:
	rm -f $(INSTALL_PREFIX)/bin/battery-notifier

.PHONY: clean
clean:
	rm -f main.o envargs.o battery-notifier
