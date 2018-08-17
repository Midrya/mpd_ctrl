#include <stdio.h>
#include <stdlib.h>
#include <mpd/client.h>
#include <unistd.h>

#define HOUR 3600
#define MINUTE 60

enum OPTION { STATUS, PREVIOUS, NEXT, TOGGLE };

void usage(FILE*);
int  song_status();
int  song_previous();
int  song_next();
int  song_toggle();
void cook_time(unsigned, unsigned*);

int
main(int argc, char *argv[])
{
	struct mpd_connection *conn = mpd_connection_new(NULL, 0, 0);
	if (argc != 2) {
		fputs("Must be called with exactly 1 argument\n", stderr);
		usage(stderr);
		exit(EXIT_FAILURE);
	}

	enum OPTION option;
	switch (getopt(argc, argv, "spnth")) {
	case 's':
		exit(song_status());
		break;
	case 'p':
		exit(song_previous());
		break;
	case 'n':
		exit(song_next());
		break;
	case 't':
		exit(song_toggle());
		break;
	case 'h':
		usage(stdout);
		exit(EXIT_SUCCESS);
		break;
	default: /* '?' */
		usage(stderr);
		exit(EXIT_FAILURE);
	}

		int exit_code;
	return 0;
}

void
usage(FILE *stream)
{
	fputs("\t-s : output song status \
        \n\t-p : go to previous song, if paused or elapsed time < 3, or restart song \
        \n\t-n : go to next song \
        \n\t-t : toggle pause/play state of song \
        \n\t-h : output help message\n",
	    stream);
}

int
song_status()
{
	unsigned time, duration;
	unsigned duration_cooked[3];
	unsigned time_cooked[3];
	struct mpd_connection *conn = mpd_connection_new(NULL, 0, 0);
	struct mpd_status *status = mpd_run_status(conn);
	struct mpd_song *song = mpd_run_current_song(conn);
	enum mpd_state state;

	if (status == NULL || song == NULL) {
		return EXIT_FAILURE;
	}

	time = mpd_status_get_elapsed_time(status);
	duration = mpd_song_get_duration(song);
	state = mpd_status_get_state(status);

	cook_time(time, time_cooked);
	cook_time(duration, duration_cooked);

	printf("%s - %s",
	    mpd_song_get_tag(song, MPD_TAG_ARTIST, 0),
	    mpd_song_get_tag(song, MPD_TAG_TITLE, 0));
	if (duration_cooked[0] > 0) {
		printf(" (%u:%02u:%02u/%u:%02u:%02u)",
		    time_cooked[0], time_cooked[1], time_cooked[2],
		    duration_cooked[0], duration_cooked[1], duration_cooked[2]);
	} else {
		printf(" (%u:%02u/%u:%02u)",
		    time_cooked[1], time_cooked[2],
		    duration_cooked[1], duration_cooked[2]);
	}
	mpd_song_free(song);
	mpd_status_free(status);
	mpd_connection_free(conn);

	return EXIT_SUCCESS;
}

void
cook_time(unsigned time, unsigned *cooked)
{
	cooked[0] = time / HOUR;
	cooked[1] = (time % HOUR) / MINUTE;
	cooked[2] = (time % MINUTE);
}

int
song_previous()
{
	struct mpd_connection *conn = mpd_connection_new(NULL, 0, 0);
	struct mpd_status *status = mpd_run_status(conn);
	enum mpd_state state = mpd_status_get_state(status);
	unsigned time = mpd_status_get_elapsed_time(status);

	if (state == MPD_STATE_UNKNOWN) {
		return EXIT_FAILURE;
	}

	if (state == MPD_STATE_PLAY && time <= 3) {
		mpd_run_previous(conn);
	}
	else if (state != MPD_STATE_PLAY) {
		mpd_run_previous(conn);
		mpd_run_toggle_pause(conn);
	}
	else {
		mpd_run_stop(conn);
		mpd_run_play(conn);
	}

	mpd_status_free(status);
	mpd_connection_free(conn);

	return EXIT_SUCCESS;
}

int
song_next()
{
	struct mpd_connection *conn = mpd_connection_new(NULL, 0, 0);
	struct mpd_status *status = mpd_run_status(conn);
	enum mpd_state state = mpd_status_get_state(status);

	if (state == MPD_STATE_UNKNOWN) {
		return EXIT_FAILURE;
	}

	if (state != MPD_STATE_PLAY) {
		mpd_run_next(conn);
		mpd_run_toggle_pause(conn);
	} else {
		mpd_run_next(conn);
	}

	mpd_connection_free(conn);

	return EXIT_SUCCESS;
}

int
song_toggle()
{
	struct mpd_connection *conn = mpd_connection_new(NULL, 0, 0);
	mpd_run_toggle_pause(conn);

	mpd_connection_free(conn);

	return EXIT_SUCCESS;
}
