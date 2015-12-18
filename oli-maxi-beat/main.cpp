/*
 * oli-maxi-beat
 *
 * Pre-programmed beats via sample morphing
 *
 * Oliver Reynolds, 2015
 * <oli_kester@zoho.com>
 *
 * Sounds best through headphones
*/

#include "maximilian.h"

//--- my methods ---
//hard limits input between 1 and -1. Otherwise values pass through to "output" unchanged
double posiClipper(double input){
    double output;

    if(input>1){
        output=1;
    } else if (input<-1) {
        output=-1;
    } else {
        output=input;
    }

    return output;
}

//--- variables ---
//sample trigger patterns - experiment with these!
int kickSeq[8]={1,1,0,1,0,0,1,0};
double kickPitch[2]={0.7,1.38};
int snareSeq[16]={0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0};
int hhSeq[6]={1,1,1,1,1,0};
int clapSeq[16]={0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,};

//samples
maxiSample kick,snare,hh,clap;

//maxi control objects
maxiClock clocker;
maxiOsc lfo1,lfo2,lfo3,lfo4;
maxiMix mixer;

//control variables - experiment with these!
int tempo= 101;
int kickModulo=7;
int snareModulo=33;
int hhModulo=5;
int clapModulo=18;

//output variables
double lfo1Out,lfo2Out,lfo3Out,lfo4Out,kickOut,snareOut,hhOut,clapOut,kickMixOut[2],snareMixOut[2],hhMixOut[2],clapMixOut[2];

void setup() {

    //load samples
    kick.load("my-kick.wav");
    snare.load("my-snare.wav");
    hh.load("my-syndrum.wav");
    clap.load("my-clap.wav");

    //set tempo
    clocker.setTempo(tempo);
    clocker.setTicksPerBeat(4);
}

void play(double *output) {
    //LFOs
    lfo1Out=lfo1.sinewave(10); //tempo independent
    lfo2Out=lfo2.phasor(clocker.bps/64); //tempo synced
    lfo3Out=lfo3.sinewave(0.7);
    lfo4Out=lfo4.square(clocker.bps/32);

    //tick clock
    clocker.ticker();

    //check each pattern array to see if it's time to trigger the sample
    if (clocker.tick) {
        if (kickSeq[clocker.playHead%(sizeof(kickSeq)/sizeof(*kickSeq))]) kick.trigger();
        if (snareSeq[clocker.playHead%(sizeof(snareSeq)/sizeof(*snareSeq))]) snare.trigger();
        if (hhSeq[clocker.playHead%(sizeof(hhSeq)/sizeof(*hhSeq))]) hh.trigger();
        if (clapSeq[clocker.playHead%(sizeof(clapSeq)/sizeof(*clapSeq))]) clap.trigger();
    }

    //modify signal on certain beats using "-modulo" values. Lower & not prime = more frequent.
    if (clocker.playHead%kickModulo)
        //normal playback
        kickOut=kick.playOnce(kickPitch[(int)(lfo4Out*0.5+1)]+lfo1Out*0.05);
    else
        //boost volume
        kickOut=kick.playOnce(kickPitch[(int)(lfo4Out*0.5+1)]+lfo1Out*0.05)*5;

    if (clocker.playHead%snareModulo)
        //normal playback
        snareOut=snare.playOnce(0.9+kickOut*0.2);
    else
        //reverse playback with pitchmod
        snareOut=snare.playOnce(-1.2+kickOut*0.2+(lfo2Out)*5);

    if (clocker.playHead%hhModulo)
        //normal playback (with pitchmod)
        hhOut=hh.playOnce(5+lfo1Out*0.5);
    else
        //reverse playback
        hhOut=hh.playOnce(-5);

    if (clocker.playHead%clapModulo)
        //modulate pitch with second LFO
        clapOut=clap.playOnce(lfo2Out+0.8)*0.4;
    else
        //modulate pitch with first LFO
        clapOut=clap.playOnce(lfo1Out+0.8)*0.4;

    //mix tracks with stereo panning
    mixer.stereo(kickOut,kickMixOut,0.5);
    mixer.stereo(snareOut,snareMixOut,0.7);
    mixer.stereo(hhOut*0.7,hhMixOut,lfo3Out*0.3+0.3);
    mixer.stereo(clapOut,clapMixOut,0.6);

    //output audio, with limiting
    output[0]=posiClipper(kickMixOut[0]+snareMixOut[0]+hhMixOut[0]+clapMixOut[0]);
    output[1]=posiClipper(kickMixOut[1]+snareMixOut[1]+hhMixOut[1]+clapMixOut[1]);
}
