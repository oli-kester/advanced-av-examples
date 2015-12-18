/* ph-zero
 *
 * An acid synth sequence with a warped 3D mesh transformation
 *
 * Oliver Reynolds, 2015 <oli_kester@zoho.com>
 *
 * Based on the "mesh" example by Mick Grierson
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

    //audio output methods
    void audioOut(float * output,int bufferSize, int nChannels);
    void audioIn(float * output,int bufferSize, int nChannels);

    //audio settings
    int bufferSize;
    int sampleRate;
    int bufferCount;

    //maxi objects
    maxiOsc saw1,lfo1,lfo2,sequencer;
    maxiFilter lowpass;

    //audio variables
    double oscFreq,fltrCut,fltrQ,ampVol,lfo1Depth,lfo1Speed,lfo2Depth,lfo2Speed,oscOut,fltrOut,keys,lfo1Out,lfo2Out,rmsOut,outSum,finalOut;

    //visual control
    double vertexMod1;
    float strength=0;

    //sequencing control
    int seqIndex;
    double notes[8]={80,50,80,50,198,40,50,182};

    //openframeworks objects
    ofSpherePrimitive sphere;
    ofVboMesh deformMesh;
    ofLight myLight;
    ofImage myTexture;
};

