=== SOUND SCAPE XY ===

An interactive soundscape.

Oliver Reynolds, 2015 <oli_kester@zoho.com>

Each of the six on-screen sections has a sample mapped. Click on each to 
trigger the sample. They can be activated in various combinations to 
change the sound. 

Whilst playing, each sample is affected in different ways by moving the
mouse over the sector.

Additionally, there are effects mapped to the keyboard - 

- hold C to trigger amplitude modulation on sample 1
- hold F to trigger pitch modulation of sample 3
- hold D to add extra delay to sample 4

--- How to Run ---

1. Place the project folder inside the "apps/myApps" folder of a valid 
openFrameworks installation (I was using the 0.9.0 release). 

Make sure you have the ofxMaxim addon installed (it's also included in 
this folder)

2. Then we have two options, either - 

	- Use Qt Creator to open the .qbs project file. You should have it 
	setup according to this guide - 
	http://openframeworks.cc/setup/qtcreator/
	
	- *** N.B. - In order to get openFrameworks & Maximilian working 
	nicely with Qt Creator on my 64-bit Linux machine , I had to make 
	the following change to my of.qbs file *** - 
	
		Substitute 
	
		".concat(['-Wno-unused-parameter','-std=gnu++14'])"
	
		with
	
		".concat(['-Wno-unused-parameter','-std=gnu++11'])"
		
		on line ~434 of your of.qbs, located in - 
		
		OF_ROOT/libs/openFrameworksCompiled/project/qtcreator/modules/of/
	
	- Hit Run on your Desktop kit (this should first activate the build 
	step, then run automatically)

OR
	
	- Use the openFrameworks Project generator to port the project to 
	your current platform  - 
	http://openframeworks.cc/tutorials/introduction/002_projectGenerator.html
	
	This method might be tricky.	
	
3. If neither of these work, you can always copy the main.cpp, ofApp.h 
and ofApp.cpp resources into a new openframeworks project. Make sure it 
includes the ofxMaxim addon. 

Failing all else, there is always my 64-bit Linux binary in the bin/ 
folder, which you are welcome to try running.

If you encounter any problems, please contact me via the email above.
