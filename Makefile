CC=clang
CFLAGS=-Wall
CLIBS=-lmpdclient
SRC=mpd_ctrl.c
INSTALLDIR=~/bin/

all: mpd_ctrl

debug: mpd_ctrl_test

mpd_ctrl_test: $(SRC)
	$(CC) -g -o $@ $^ $(CFLAGS) $(CLIBS)

mpd_ctrl: mpd_ctrl.c
	$(CC) -o $@ $^ $(CFLAGS) $(CLIBS)

install: all
	cp mpd_ctrl $(INSTALLDIR)mpd_ctrl

tags:
	ctags -f TAGS --c-kinds=+p -e -R $(pwd)

clean:
	rm mpd_ctrl mpd_ctrl_test

uninstall:
	rm $(INSTALLDIR)mpd_ctrl
