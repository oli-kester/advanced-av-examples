/* ph-zero
 *
 * An acid synth sequence with a warped 3D mesh transformation
 *
 * Oliver Reynolds, 2015 <oli_kester@zoho.com>
 *
 * Based on the "mesh" OF example by Mick Grierson
 *
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //audio settings
    sampleRate = 44100;
    bufferSize = 512;
    bufferCount=0;

    //initialise control values
    oscFreq=50;
    fltrCut=50;
    fltrQ=3;
    lfo1Depth=2000;
    lfo1Speed=6;
    lfo2Depth=0.7;
    lfo2Speed=5;
    ampVol=0.5;
    keys=50;

    //OF window setup
    ofBackground(0);
    ofSetWindowTitle("ph-zero");

    //setup our plane
    sphere.set(300, 300);
    sphere.setResolution(50);
    sphere.setPosition(ofGetWidth()*0.5, ofGetHeight()*0.5, 0);
    sphere.rotate(45,1,0,0);

    //set the light to directional mode
    myLight.setDirectional();

    //load the texture from disk, and set it to repeat when wrapped
    myTexture.load(ofToDataPath("/home/oliver/Documents/uni/year4/av-processing/of_v0.9.0_linux64_release/apps/myApps/ph-zero/graf.png"));
    myTexture.getTexture().setTextureWrap( GL_REPEAT, GL_REPEAT );

    //set sound output
    ofSoundStreamSetup(2,2,this,sampleRate,bufferSize,4);
}

//--------------------------------------------------------------
void ofApp::update(){
    //rotate the sphere to keep it interesting
    sphere.rotate(0.1,0.2,0.5,0);

    //aim the light straight at it
    myLight.setOrientation(ofVec3f(0,180,0));

    //modify synth parameters with the mouse
    fltrCut=mouseX*5-200;
    lfo1Speed=mouseY*0.01;
}

//--------------------------------------------------------------
void ofApp::draw(){
    //render in z-plane order with depth testing
    ofEnableDepthTest();

    // bathe in light
    ofEnableLighting();
    myLight.enable();

    //wrap the sphere with the loaded texture
    myTexture.getTexture().bind();
    sphere.mapTexCoordsFromTexture( myTexture.getTexture() );

    //get the mesh of the sphere
    deformMesh = sphere.getMesh();

    //variable to store vertex coordinates
    ofVec3f currentVertex;

    //loop through all verticies contained in the sphere
    for(int i = 0; i < deformMesh.getNumIndices(); i++ ) {

        //this variable is set very low to control the strength of
        strength = 0.000002;

        //grab the index of this vertex
        int vertexIndex = deformMesh.getIndex(i);
        //get the vertex accordingly
        currentVertex = deformMesh.getVertex(vertexIndex);

        //make new coordinates from the mouse position
        float newX = cos(cos(mouseX*strength*i));
        float newY = cos(sin(mouseY*strength*i));

        //multiply the current coordinates by them
        currentVertex.x *= newX;
        currentVertex.y *= (newY*(rmsOut+1)*1.2)-0.2; //Y plane is multiplied by the audio RMS
        currentVertex.z *= newX*newY;

        //write back the new (deformed) coordinates
        deformMesh.setVertex( vertexIndex, currentVertex );

    }
    sphere.transformGL();

    //Now we draw our newly deformed plane
    deformMesh.draw();

    //unbind the loaded texture
    myTexture.getTexture().unbind();

    ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //save last pressed key
    keys=key;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::audioOut(float * output,int bufferSize, int nChannels){

    //buffer counts for RMS calculation
    bufferCount++;
    outSum=0;

    for (int i=0;i<bufferSize;i++){

        //get LFO values
        lfo1Out=lfo1.sinewave(lfo1Speed)*lfo1Depth+lfo1Depth;
        lfo2Out=lfo2.sinewave(lfo2Speed)*lfo2Depth;

        //use a phasor to step through the sequence
        seqIndex=sequencer.phasor(0.75,1,8);

        //get the sound from a saw oscillator. Notes come from an
        //array, and are scaled by the last pressed key value
        oscOut=saw1.sawn(notes[seqIndex]+lfo2Out+(keys-50)*5);

        //filter the raw wave
        fltrOut=lowpass.lores(oscOut,fltrCut+lfo1Out,fltrQ);

        //modify amplitude
        finalOut=fltrOut*ampVol;

        //add output to RMS sum
        outSum+=fabs(finalOut);

        //send audio out
        output[i*nChannels]=finalOut;
        output[i*nChannels+1]=finalOut;
    }

    //calculate RMS in 15 frame blocks
    if (bufferCount>=15){
        rmsOut=sqrt(outSum/(bufferSize*15));
        bufferCount=0;
        vertexMod1=lfo1Out;
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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

