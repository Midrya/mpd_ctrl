CC=clang

mpd_controller: mpd_controller.c
	$(CC) -o mpd_controller -lmpdclient mpd_controller.c

