/*
 * amp-osc.lv2 - an LV2 amplitude oscillator
 *
 * Based on the Simple amplifier example (amp.c) by David Robillard
 * https://github.com/drobilla/lv2/tree/master/plugins/eg-amp.lv2
 *
 * Ported to C++
 *
 * Contains synth functions and PI values from the Maximilian library
 * http://maximilian.strangeloop.co.uk/
 *
 * Copyright 2015 Oliver Reynolds <oli_kester@zoho.com>
 *
  Copyright 2006-2011 David Robillard <d@drobilla.net>
  Copyright 2006 Steve Harris <steve@plugin.org.uk>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <math.h>
#include <stdlib.h>
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>

//pi values from maximilian.h
#ifndef PI
#define PI  3.1415926535897932384626433832795
#endif
#define TWOPI 6.283185307179586476925286766559

#define AMPOSC_URI "oli_kester/amp-osc"

//define enum for port indicies
typedef enum {
    AMPOSC_FREQUENCY  = 0,
    AMPOSC_SINELEVEL   = 1,
    AMPOSC_SAWLEVEL   = 2,
    AMPOSC_SQUARELEVEL   = 3,
    AMPOSC_DRYWET   = 4,
    AMPOSC_INPUT  = 5,
    AMPOSC_OUTPUT = 6
} PortIndex;

//define plugin struct
typedef struct {
    //I/O
    const float* frequency;
    const float* sinelevel;
    const float* sawlevel;
    const float* squarelevel;
    const float* drywet;
    // Port buffers
    const float* input;
    float*       output;
    //variables
    //separate phase values to keep waves synced
    double sinePhase,sawPhase,sqrPhase,sampleRate;
} AmpOsc;

//--- wave classes adapted from maximilian ---

double sine(LV2_Handle instance, double frequency) {
    AmpOsc* thisAmpOsc = (AmpOsc*)instance;
    //This is a sinewave oscillator
    double output=sin(thisAmpOsc->sinePhase*(TWOPI));
    if ( thisAmpOsc->sinePhase >= 1.0 )
        thisAmpOsc->sinePhase -= 1.0;
    thisAmpOsc->sinePhase += (1./(thisAmpOsc->sampleRate/(frequency)));
    return(output);
}

double saw(LV2_Handle instance, double frequency) {
    AmpOsc* thisAmpOsc = (AmpOsc*)instance;
    //Sawtooth generator. This is like a phasor but goes between -1 and 1
    double output=thisAmpOsc->sawPhase;
    if ( thisAmpOsc->sawPhase >= 1.0 )
        thisAmpOsc->sawPhase -= 2.0;
    thisAmpOsc->sawPhase += (1./(thisAmpOsc->sampleRate/(frequency)));
    return(output);
}

double square(LV2_Handle instance, double frequency) {
    AmpOsc* thisAmpOsc = (AmpOsc*)instance;
    double output;
    //This is a square wave
    if (thisAmpOsc->sqrPhase<0.5)
        output=-1;
    if (thisAmpOsc->sqrPhase>0.5)
        output=1;
    if ( thisAmpOsc->sqrPhase >= 1.0 )
        thisAmpOsc->sqrPhase -= 1.0;
    thisAmpOsc->sqrPhase += (1./(thisAmpOsc->sampleRate/(frequency)));
    return(output);
}

//hard limits input between 1 and 0. Otherwise values pass through to "output" unchanged
double posiClipper(double input){
    double output;

    if(input>1){
        output=1;
    } else if (input<0) {
        output=0;
    } else {
        output=input;
    }
    return output;
}

//create new plugin instance
static LV2_Handle instantiate(
        const LV2_Descriptor*     descriptor,
        double                    rate,
        const char*               bundle_path,
        const LV2_Feature* const* features)
{
    AmpOsc* thisAmpOsc = (AmpOsc*)malloc(sizeof(AmpOsc));

    //save sample rate
    thisAmpOsc->sampleRate=rate;

    return (LV2_Handle)thisAmpOsc;
}

//connect ports
static void connect_port(
        LV2_Handle  instance,
        uint32_t    port,
        void*       data)
{
    AmpOsc* thisAmpOsc = (AmpOsc*)instance;

    switch ((PortIndex)port) {
    case AMPOSC_FREQUENCY:
        thisAmpOsc->frequency = (const float*)data;
        break;
    case AMPOSC_SINELEVEL:
        thisAmpOsc->sinelevel = (const float*)data;
        break;
    case AMPOSC_SAWLEVEL:
        thisAmpOsc->sawlevel = (const float*)data;
        break;
    case AMPOSC_SQUARELEVEL:
        thisAmpOsc->squarelevel = (const float*)data;
        break;
    case AMPOSC_DRYWET:
        thisAmpOsc->drywet = (const float*)data;
        break;
    case AMPOSC_INPUT:
        thisAmpOsc->input = (const float*)data;
        break;
    case AMPOSC_OUTPUT:
        thisAmpOsc->output = (float*)data;
        break;
    }
}

//activate/reset plugin
static void activate(LV2_Handle instance)
{
    AmpOsc* thisAmpOsc = (AmpOsc*)instance;

    //reset phases
    thisAmpOsc->sinePhase=0;
    thisAmpOsc->sawPhase=0;
    thisAmpOsc->sqrPhase=0;
}

//run processing on samples of audio
static void run(LV2_Handle instance, uint32_t n_samples)
{
    AmpOsc* thisAmpOsc = (AmpOsc*)instance;

    const float        frequency   = *(thisAmpOsc->frequency);
    const float        sinelevel   = *(thisAmpOsc->sinelevel);
    const float        sawlevel   = *(thisAmpOsc->sawlevel);
    const float        squarelevel   = *(thisAmpOsc->squarelevel);
    const float        drywet   = *(thisAmpOsc->drywet);
    const float* const input  = thisAmpOsc->input;
    float* const       output = thisAmpOsc->output;

    for (uint32_t pos = 0; pos < n_samples; pos++) {
        //temporary storage variables
        double sineOut,sawOut,sqrOut,modWave,dryOut,wetOut;

        //generate (only positive) values for each wave
        sineOut=(sine((AmpOsc*)thisAmpOsc,frequency)+1)*0.5;
        sawOut=(saw((AmpOsc*)thisAmpOsc,frequency)+1)*0.5;
        sqrOut=(square((AmpOsc*)thisAmpOsc,frequency)+1)*0.5;

        //add them together by a factor of their volume to get the final modulating wave
        modWave=sineOut*sinelevel+sawOut*sawlevel+sqrOut*squarelevel;

        //hard limit modwave
        modWave=posiClipper(modWave);

        //dry output level
        dryOut=input[pos]*(1-drywet*0.01);

        //modulate input to get wet signal level
        wetOut=input[pos]*modWave*drywet*0.01;

        //mix output
        output[pos] = dryOut+wetOut;
    }
}

//deactivate - not used here
static void deactivate(LV2_Handle instance)
{
}

//destroy instance (free the memory)
static void cleanup(LV2_Handle instance)
{
    free(instance);
}

//extensions - not used here
static const void* extension_data(const char* uri)
{
    return NULL;
}

//return descriptor using the URI defined at the top
static const LV2_Descriptor descriptor = {
    AMPOSC_URI,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};

// return descriptor (plugin entry point)
LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
    switch (index) {
    case 0:  return &descriptor;
    default: return NULL;
    }
}
