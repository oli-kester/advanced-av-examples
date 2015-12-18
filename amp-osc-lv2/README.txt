==== amp-osc.lv2 ====

A amplitude modulation LV2 plugin, for use with LV2 compatible hosts. 

Oliver Reynolds, 2015 <oli_kester@zoho.com>

There seems to be no Linux-compatible amplitude modulation plugin, so
I decided to make one! Has three variable oscillators - sine, saw and
square, which modulate the amplitude of the input signal. The modulation
frequency is set with the "Frequency (Hz)" slider, and the output is
continuously variable between the dry and wet signals using the 
"Dry/Wet" slider.

It is possible to have more than one wave activated, and to have them
modulate the signal simultaneously!

The sine wave modulates VERY nicely. The saw and square waves work, but 
can produce artifacts at high wet levels. Be careful.

The wave functions are taken from Maximilian - 
http://maximilian.strangeloop.co.uk/

Builds against the LV2 library (included). For information about the
library, see the LV2 website - 
http://lv2plug.in/pages/developing.html

For a list of compatible hosts and other plugins, see - 
http://lv2plug.in/pages/projects.html

The plugin constitutes three files - 
	- amp-osc.cpp
	- manifest.ttl.in
	- amp-osc.ttl
	
All processing is done in the .cpp file. The two .ttl files are static
definitions - used by hosts to locate and define the plugin.

My Qt Creator .creator project is also included.

--- To Build ---

Prerequisites - 

	- A recent Python installation
	
	- An LV2 host application
	
To build, from this directory - 

	1. ./waf configure

	2. ./waf

        3. There is now an amp-osc.lv2/ folder in the build/ directory.
	Copy this to your LV2 plugins directory (e.g. "~/.lv2/")
	
	4. Run your plugin host, making sure that it is searching the plugin
	directory you chose in step 3
	
	5. Add the plugin to your tracks!
	
Tested & working with Ardour and Qtractor. If you encounter any problems, 
please contact me via the email above.
