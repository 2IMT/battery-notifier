CC ?= gcc
CFLAGS = -std=c17 -Wall -Wextra
LDFLAGS =
MYCFLAGS =

INSTALL_PREFIX = /usr/local

all: battery-notifier

battery-notifier: main.o envargs.o battery.o
	$(CC) $(CFLAGS) $(MYCFLAGS) -o battery-notifier main.o envargs.o battery.o

main.o: main.c envargs.h battery.h
	$(CC) $(CFLAGS) $(MYCFLAGS) $(LDFLAGS) -c main.c

envargs.o: envargs.c envargs.h util.h
	$(CC) $(CFLAGS) $(MYCFLAGS) $(LDFLAGS) -c envargs.c

battery.o: battery.c battery.h util.h
	$(CC) $(CFLAGS) $(MYCFLAGS) $(LDFLAGS) -c battery.c

.PHONY: install
install: battery-notifier
	install -Dm755 battery-notifier $(INSTALL_PREFIX)/bin/battery-notifier

.PHONY: uninstall
uninstall:
	rm -f $(INSTALL_PREFIX)/bin/battery-notifier

.PHONY: clean
clean:
	rm -f main.o envargs.o battery.o battery-notifier
