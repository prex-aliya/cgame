/** \file sound.c
 ** \author prex-aliya
 ** \breif sound controller
 **
 ** This is the sound controller for this project.
 ** Currently only does sinewaves, but would love to expand
 ** for playing audio files.
 **  Also this is taken from PortAudios examples_src group, and originaly
 ** Only played a sine wave.
 ** */

#include <stdio.h>
#include <math.h>
#include <portaudio.h>

#define FRAMES (64)

// If M_PI is not defined, then define it.
#ifndef M_PI
#define M_PI (3.1415926539)
#endif

#define TABLE_SIZE (300)
typedef struct {
    float sine[TABLE_SIZE];
    int left_phase;
    int right_phase;
    char message[20];
} paTestData;

/* This is called by PortAudio, for audio; and such,
 * needs all the arguments even if they are not used.
 * NOTE: Do not use malloc() or free() in this function
 */
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData) {
    /* Remove Compiler Error */
    (void)timeInfo; (void)statusFlags; (void)inputBuffer;
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i;


    for (i=0; i<framesPerBuffer; i++) {
        *out++ = data->sine[data->left_phase];  /* Left */
        *out++ = data->sine[data->right_phase]; /* Right */
        data->right_phase += 1;
        if (data->left_phase >= TABLE_SIZE) data->left_phase -= TABLE_SIZE;
        data->right_phase += 2; /* Higher Pitch */
        if (data->right_phase >= TABLE_SIZE) data->right_phase -= TABLE_SIZE;
    }

    return paContinue;
}


/* This is called when playback is done */
static void StreamFinished (void* userData) {
    // paTestData *data = (paTestData *) userData;
    // "AUDIO: Stream Completed: %s" data->message
    // TODO: implement logging system.
}

// TODO: have pointer instead of passing var
PaError error(PaError err) {
    Pa_Terminate();
    // TODO log file
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}

int sine(int time, int sample) {

    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    paTestData data;
    int i;

    // "AUDIO: output sine wave. SR = %d, BufSize = %d", sample_rate, FRAMES

    /* Initizlise sinusoidal wavetable */
    for (i=0; i<TABLE_SIZE; i++)
        data.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );

    data.left_phase = data.right_phase = 0;

    err = Pa_Initialize();
    if (err != paNoError) goto error;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
      //fprintf(stderr,"Error: No default output device.\n");
       goto error;
    }
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              NULL, /* no input */
              &outputParameters,
              sample,
              FRAMES,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              patestCallback,
              &data );
    if( err != paNoError ) goto error;

    //sprintf( data.message, "No Message" );
    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    Pa_Sleep(time);

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();
    //printf("Test finished.\n");

    return err;

error:
    Pa_Terminate();
    //fprintf( stderr, "An error occured while using the portaudio stream\n" );
    //fprintf( stderr, "Error number: %d\n", err );
    //fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}

int default_sine() {
    sine(100, 35000);
    return 0;
}
