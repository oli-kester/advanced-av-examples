/*
 * An interactive soundscape
 *
 * Oliver Reynolds, 2015
 *
 * <oli_kester@zoho.com>
 *
 */

#include "ofApp.h"

//--- my methods ---
//hard limits input between 1 and -1. Otherwise values pass through to "output" unchanged
double ofApp::clipper(double input){
    double output;

    if(input>1)
        output=1;

    else if (input<-1)
        output=-1;

    else
        output=input;

    return output;
}

//--------------------------------------------------------------
void ofApp::setup(){
    //setup audio
    sampleRate = 44100;
    bufferSize = 512;

    //init key value
    lastKey=0;

    //init counter
    counter=0;

    //init control values
    triggerRate=10;
    sector3MouseOver=true;
    bpCutoff=10000;
    hpCutoff=0.85;

    //load samples
    samples[0].load("../samples/bass-crickets.wav");
    samples[1].load("../samples/chasm-pinging-echoes.wav");
    samples[2].load("../samples/classy-cal-loop.wav");
    samples[3].load("../samples/skin-crawl.wav");
    samples[4].load("../samples/tunnel-amb.wav");
    samples[5].load("../samples/undersea-manta.wav");

    //auto trim & normalise samples
    for (int i=0;i<=5;i++){
        samples[i].autoTrim();
        samples[i].normalise();
    }

    //setSampleMuting
    sample1Mute=true;
    sample2Mute=true;
    sample3Mute=true;
    sample4Mute=true;
    sample5Mute=true;
    sample6Mute=true;

    //set sample playback speeds
    sample1Pitch=0.8;
    sample2Pitch=0.9;
    sample3Pitch=0.1;
    sample4Pitch=0.8;
    sample5Pitch=0.2;
    sample6Pitch=2;

    //set sample panning
    sample1Pan=0.2;
    sample2Pan=0.4;
    sample3Pan=0.8;
    sample4Pan=0.5;
    sample5Pan=0.4;
    sample6Pan=0.8;

    //set sample volume
    sample1Volume=0.7;
    sample2Volume=0.2;
    sample3Volume=1;
    sample4Volume=0.15;
    sample5Volume=0.5;
    sample6Volume=0.15;

    //number of triggers to hold FX for
    hold1 = 50;
    hold2 = 50;
    hold3 = 50;
    hold4 = 50;
    hold5 = 50;
    hold6 = 50;

    //setup visuals
    ofSetFrameRate(60);
    ofSetWindowTitle("sound-scape-xy");
    ofBackground(0);

    //setup sound output
    ofSoundStreamSetup(2,2,this,sampleRate,bufferSize,4);
}

//--------------------------------------------------------------
void ofApp::update(){
    //increment frame counter
    counter++;

    //DEBUG uncomment to output mouse position
    //        cout << "mouseX - " << mouseX << endl << "mouseY - " << mouseY << endl;

    //only perform actions here every "triggerRate" frames
    if (counter==triggerRate){

        //affect samples when mouse is in a sector.
        //top-left (section 1)
        if (mouseX<ofGetWidth()*0.5 && mouseY<ofGetHeight()*(1./3.)){
            sector1MouseOver=true;
        } else {
            sector1MouseOver=false;
        }
        //top-right
        if (mouseX>ofGetWidth()*0.5 && mouseY<ofGetHeight()*(1./3.)){
            //retrigger
            sector2MouseOver=true;
            samples[1].setPosition(1000);
            samples[1].trigger();
        } else
            sector2MouseOver=false;
        //centre-left
        if (mouseX<ofGetWidth()*0.5 && mouseY>ofGetHeight()*(1./3.) && mouseY<ofGetHeight()*(2./3.)){
            //disable bandpass & speed up
            sector3MouseOver=true;
            sample3Pitch=0.4;
            sample3Volume=0.65;
        } else {
            //reset
            sector3MouseOver=false;
            sample3Pitch=0.1;
            sample3Volume=1;
        }
        //centre-right
        if (mouseX>ofGetWidth()*0.5 && mouseY>ofGetHeight()*(1./3.) && mouseY<ofGetHeight()*(2./3.)){
            //TODO mouseover4
            sector4MouseOver=true;
        } else
            sector4MouseOver=false;
        //bottom-left
        if (mouseX<ofGetWidth()*0.5 && mouseY>ofGetHeight()*(2./3.) && mouseY<ofGetHeight()){
            //TODO mouseover5
            sector5MouseOver=true;
        } else
            sector5MouseOver=false;
        //bottom-right
        if (mouseX>ofGetWidth()*0.5 && mouseY>ofGetHeight()*(2./3.) && mouseY<ofGetHeight()){
            //play in reverse
            sample6Pitch=1.5;
            sector6MouseOver=true;
        } else {
            sample6Pitch=2;
            sector6MouseOver=false;
        }
        //reset counter
        counter=0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //draw lines deliniating sector boundaries
    ofSetColor(150,0,110,230);
    ofSetLineWidth(3);
    //centre
    ofDrawLine(ofGetWidth()*0.5,0,ofGetWidth()*0.5,ofGetHeight());
    //top third
    ofDrawLine(0,ofGetHeight()*(1./3.),ofGetWidth(),ofGetHeight()*(1./3.));
    //bottom third
    ofDrawLine(0,ofGetHeight()*(2./3.),ofGetWidth(),ofGetHeight()*(2./3.));

    //show mute & mouseover status by drawing rectangles
    if (!sample1Mute){
        if (sector1MouseOver){
            //green
            ofSetColor(0,125,40,150);
        } else {
            //gray
            ofSetColor(255,150);
        }
        ofDrawRectRounded(10,10,ofGetWidth()*0.5-20,ofGetHeight()*(1./3.)-20,5);
    }
    if (!sample2Mute){
        if (sector2MouseOver){
            ofSetColor(0,125,40,150);
        } else {
            ofSetColor(255,150);
        }
        ofDrawRectRounded(ofGetWidth()*0.5+10,10,ofGetWidth()*0.5-20,ofGetHeight()*(1./3.)-20,5);
    }
    if (!sample3Mute){
        if (sector3MouseOver){
            ofSetColor(0,125,40,150);
        } else {
            ofSetColor(255,150);
        }
        ofDrawRectRounded(10,ofGetHeight()*(1./3.)+10,ofGetWidth()*0.5-20,ofGetHeight()*(1./3.)-20,5);
    }
    if (!sample4Mute){
        if (sector4MouseOver){
            ofSetColor(0,125,40,150);
        } else {
            ofSetColor(255,150);
        }
        ofDrawRectRounded(ofGetWidth()*0.5+10,ofGetHeight()*(1./3.)+10,ofGetWidth()*0.5-20,ofGetHeight()*(1./3.)-20,5);
    }
    if (!sample5Mute){
        if (sector5MouseOver){
            ofSetColor(0,125,40,150);
        } else {
            ofSetColor(255,150);
        }
        ofDrawRectRounded(10,ofGetHeight()*(2./3.)+10,ofGetWidth()*0.5-20,ofGetHeight()*(1./3.)-20,5);
    }
    if (!sample6Mute){
        if (sector6MouseOver){
            ofSetColor(0,125,40,150);
        } else {
            ofSetColor(255,150);
        }
        ofDrawRectRounded(ofGetWidth()*0.5+10,ofGetHeight()*(2./3.)+10,ofGetWidth()*0.5-20,ofGetHeight()*(1./3.)-20,5);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //save the last pressed key
    lastKey=key;

    //DEBUG uncomment to output key presses
//    cout << "key press - " << key << endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    //reset keys on a release
    lastKey=0;
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output,int bufferSize, int nChannels){

    for (int i=0;i<bufferSize;i++){
        //play samples. Play differently with a mouseover
        //don't play if the sample is muted.
        //Any conditions here retrigger much faster than in the "update" method
        if (!sample1Mute) {
            if (sector1MouseOver){
                //playback with pitch modulation
                sample1Pitch-=(myMod3.sinewave(0.01)+1)*0.05;
                sample1Volume=0.05;
            } else {
                //reset
                sample1Volume=0.7;
                sample1Pitch=0.8;
            }
            //play the sound
            sample1Out=samples[0].play(sample1Pitch)*sample1Volume;
        }

        if (!sample2Mute)
            sample2Out=samples[1].play(sample2Pitch)*sample2Volume;

        if (!sample3Mute)
            sample3Out=samples[2].play(sample3Pitch)*sample3Volume;

        if (!sample4Mute)
            sample4Out=samples[3].play(sample4Pitch)*sample4Volume;

        if (!sample5Mute)
            sample5Out=samples[4].play(sample5Pitch)*sample5Volume;

        if (!sample6Mute){
            if (sector6MouseOver){
                //play in reverse
                sample6Out=samples[5].play(-sample6Pitch)*sample6Volume;
            } else {
                //play forwards
                sample6Out=samples[5].play(sample6Pitch)*sample6Volume;
            }
        }

        //--- FX ---
        // activated with keys or mouseovers
        if (!sector3MouseOver){
            //activate bandpass
            sample3Out=myBPF.bandpass(sample3Out,bpCutoff,0);
        }
        //high pass filter
        if (sector4MouseOver){
            sample4Out=myHPF.hipass(sample4Out,hpCutoff)*2;
        }
        //oscillating resonant lowpass
        if (sector5MouseOver){
            sample5Out=myLoRes.lores(sample5Out,100-((myMod4.sinewave(0.2)+1)*3),2);
        }
        //C key triggers fast ampmod
        if (lastKey==99){
            sample1Out*=myMod1.sinewave(9);
        }
        //F key triggers slow pitchmod
        if (lastKey==102){
            sample3Pitch=0.1*(myMod2.sinewave(0.3)+2)*0.25;
            sample3Out*=1.5;
        }
        //D key triggers extra delay
        if (lastKey==100){
            sample4Out=myDelay.dl(sample4Out,5,0.55,5);
        } else {
            sample4Out=myDelay.dl(sample4Out,1,0.3,5);
        }

        //--- mixing ---
        mixer.stereo(sample1Out,sample1Mix,sample1Pan);
        mixer.stereo(sample2Out,sample2Mix,sample2Pan);
        mixer.stereo(sample3Out,sample3Mix,sample3Pan);
        mixer.stereo(sample4Out,sample4Mix,sample4Pan);
        mixer.stereo(sample5Out,sample5Mix,sample5Pan);
        mixer.stereo(sample6Out,sample6Mix,sample6Pan);
        finalMix[0]=sample1Mix[0]+sample2Mix[0]+sample3Mix[0]+sample4Mix[0]+sample5Mix[0]+sample6Mix[0];
        finalMix[1]=sample1Mix[1]+sample2Mix[1]+sample3Mix[1]+sample4Mix[1]+sample5Mix[1]+sample6Mix[1];

        //hard limit output
        finalMix[0]=clipper(finalMix[0]);
        finalMix[1]=clipper(finalMix[1]);

        //send to buffers
        output[i*nChannels]=finalMix[0];
        output[i*nChannels+1]=finalMix[1];
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(float * output,int bufferSize, int nChannels){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //mute/unmute samples by clicking each sector
    if (x<ofGetWidth()*0.5 && y<ofGetHeight()*(1./3.)){
        sample1Mute=!sample1Mute;
    }
    //top-right
    if (x>ofGetWidth()*0.5 && y<ofGetHeight()*(1./3.)){
        sample2Mute=!sample2Mute;
    }
    //centre-left
    if (x<ofGetWidth()*0.5 && y>ofGetHeight()*(1./3.) && y<ofGetHeight()*(2./3.)){
        sample3Mute=!sample3Mute;
    }
    //centre-right
    if (x>ofGetWidth()*0.5 && y>ofGetHeight()*(1./3.) && y<ofGetHeight()*(2./3.)){
        sample4Mute=!sample4Mute;
    }
    //bottom-left
    if (x<ofGetWidth()*0.5 && y>ofGetHeight()*(2./3.) && y<ofGetHeight()){
        sample5Mute=!sample5Mute;
    }
    //bottom-right
    if (x>ofGetWidth()*0.5 && y>ofGetHeight()*(2./3.) && y<ofGetHeight()){
        sample6Mute=!sample6Mute;
    }

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    //DEBUG uncomment to output window size on resize
    //    cout << "windowX - " << w << endl << "windowY - " << h << endl;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

