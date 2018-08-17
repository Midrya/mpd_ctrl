CC=clang

test: mpd_controller.c
	$(CC) -g -o mpd_controller_test -lmpdclient mpd_controller.c

build: mpd_controller.c
	$(CC) -o mpd_controller -lmpdclient mpd_controller.c

