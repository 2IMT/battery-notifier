VERSION = 1.0

CC ?= gcc
CFLAGS = -Wall -Wextra
MYCFLAGS =
LDFLAGS =
DEFINITIONS = -DBN_VERSION=\"$(VERSION)\"
MYDEFINITIONS =

INSTALL_PREFIX = /usr/local

all: battery-notifier

battery-notifier: main.o envargs.o battery.o notify.o
	$(CC) $(CFLAGS) $(MYCFLAGS) $(LDFLAGS) -o battery-notifier main.o envargs.o battery.o notify.o

main.o: main.c envargs.h battery.h notify.h
	$(CC) $(CFLAGS) $(MYCFLAGS) $(DEFINITIONS) $(MYDEFINITIONS) -c main.c

envargs.o: envargs.c envargs.h util.h
	$(CC) $(CFLAGS) $(MYCFLAGS) $(DEFINITIONS) $(MYDEFINITIONS) -c envargs.c

battery.o: battery.c battery.h util.h
	$(CC) $(CFLAGS) $(MYCFLAGS) $(DEFINITIONS) $(MYDEFINITIONS) -c battery.c

notify.o: notify.c notify.h
	$(CC) $(CFLAGS) $(MYCFLAGS) $(DEFINITIONS) $(MYDEFINITIONS) -c notify.c

.PHONY: install
install: battery-notifier
	install -Dm755 battery-notifier $(INSTALL_PREFIX)/bin/battery-notifier

.PHONY: uninstall
uninstall:
	rm -f $(INSTALL_PREFIX)/bin/battery-notifier

.PHONY: clean
clean:
	rm -f main.o envargs.o battery.o notify.o battery-notifier
