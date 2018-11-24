
// FFT Test
//
// Compute a 1024 point Fast Fourier Transform (spectrum analysis)
// on audio connected to the Left Line-In pin.  By changing code,
// a synthetic sine wave can be input instead.
//
// The first 40 (of 512) frequency analysis bins are printed to
// the Arduino Serial Monitor.  Viewing the raw data can help you
// understand how the FFT works and what results to expect when
// using the data to control LEDs, motors, or other fun things!
//
// This example code is in the public domain.

// JJ test edit

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioInputI2S          audioInput;         // audio shield: mic or line-in
AudioSynthWaveformSine sinewave;
AudioAnalyzeFFT1024    myFFT;
AudioOutputI2S         audioOutput;        // audio shield: headphones & line-out

// Connect either the live input or synthesized sine wave
AudioConnection patchCord1(audioInput, 0, myFFT, 0);
//AudioConnection patchCord1(sinewave, 0, myFFT, 0);

AudioControlSGTL5000 audioShield;

// An array to hold the 16 frequency bands
float level[16];

void setup() 
{
    // Audio connections require memory to work.  For more
    // detailed information, see the MemoryAndCpuUsage example
    AudioMemory(12);

    // Enable the audio shield and set the output volume.
    audioShield.enable();
    audioShield.inputSelect(myInput);
    audioShield.volume(0.5);

    // Configure the window algorithm to use
    myFFT.windowFunction(AudioWindowHanning1024);
    //myFFT.windowFunction(NULL);

    // Create a synthetic sine wave, for testing
    // To use this, edit the connections above
    sinewave.amplitude(0.8);
    sinewave.frequency(1034.007);
}

void loop() 
{
    if (myFFT.available()) 
    {
        // read the 512 FFT frequencies into 16 levels
        // music is heard in octaves, but the FFT data
        // is linear, so for the higher octaves, read
        // many FFT bins together.
        level[0] =  myFFT.read(0);
        level[1] =  myFFT.read(1);
        level[2] =  myFFT.read(2, 3);
        level[3] =  myFFT.read(4, 6);
        level[4] =  myFFT.read(7, 10);
        level[5] =  myFFT.read(11, 15);
        level[6] =  myFFT.read(16, 22);
        level[7] =  myFFT.read(23, 32);
        level[8] =  myFFT.read(33, 46);
        level[9] =  myFFT.read(47, 66);
        level[10] = myFFT.read(67, 93);
        level[11] = myFFT.read(94, 131);
        level[12] = myFFT.read(132, 184);
        level[13] = myFFT.read(185, 257);
        level[14] = myFFT.read(258, 359);
        level[15] = myFFT.read(360, 511);

        for (uint8_t i = 0; i < 16; i++) 
        {
            Serial.print(level[i]);
            Serial.print("  ");
        }

        Serial.println();
    }
}