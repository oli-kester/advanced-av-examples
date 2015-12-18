==== oli-maxi-beat ====

Pre-programmed beats via sample morphing

Oliver Reynolds, 2015 <oli_kester@zoho.com>

Sounds best through headphones

--- To Build ---

*** See the adjacent Maximilian "README" file for more detail ***

On Linux with Jack audio, the build steps were - 

1. g++ -Wall -D__UNIX_JACK__ -o oli-maxi-beat main.cpp RtAudio.cpp player.cpp maximilian.cpp `pkg-config --cflags --libs jack` -lpthread

2. ./oli-maxi-beat

For other platforms, instructions can be found in the "README" file. 

If you encounter any problems, please contact me via the email above.
