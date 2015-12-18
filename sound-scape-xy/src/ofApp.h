/*
 * An interactive soundscape
 *
 * Oliver Reynolds, 2015
 *
 * <oli_kester@zoho.com>
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void audioOut(float * output,int bufferSize, int nChannels);
    void audioIn(float * output,int bufferSize, int nChannels);

    //my functions
    double clipper(double input);

    //audio setup
    int bufferSize;
    int sampleRate;

    //maxi objects
    maxiSample samples[6];
    maxiOsc mySawn,myPulse,lfoPhasor,lfoSine1,lfoSine2;
    maxiFilter myHPF,myBPF,myLoRes;
    maxiEnv myEnv1;
    maxiDelayline myDelay;
    maxiOsc myMod1, myMod2, myMod3,myMod4;
    maxiMix mixer;

    //last key pressed
    int lastKey;

    //control parameters
    int counter,triggerRate;
    double bpCutoff,hpCutoff;

    //function enable/disable
    bool sector1MouseOver,sector2MouseOver,sector3MouseOver,sector4MouseOver,sector5MouseOver,sector6MouseOver;

    //sample muting
    bool sample1Mute,sample2Mute,sample3Mute,sample4Mute,sample5Mute,sample6Mute;

    //playback speeds
    double sample1Pitch,sample2Pitch,sample3Pitch,sample4Pitch,sample5Pitch,sample6Pitch;

    //panning
    double sample1Pan,sample2Pan,sample3Pan,sample4Pan,sample5Pan,sample6Pan;

    //volume
    double sample1Volume,sample2Volume,sample3Volume,sample4Volume,sample5Volume,sample6Volume;

    //number of triggers to hold sector FX for
    int hold1,hold2,hold3,hold4,hold5,hold6;

    //audio output variables
    double sample1Out,sample2Out,sample3Out,sample4Out,sample5Out,sample6Out,
    sample1Mix[2],sample2Mix[2],sample3Mix[2],sample4Mix[2],sample5Mix[2],sample6Mix[2],
    finalMix[2];
};

