# mpd_ctrl
Small MPD controller, designed for use in shell scripts

Includes basic functionality for controlling a local mpd server.
    -s : output song status
    -p[num] : go to previous song, if paused or elapsed time < num seconds, or restart song. num defaults to 3.
    -n : go to next song
    -t : toggle pause/play state of song
    -h : output help message
