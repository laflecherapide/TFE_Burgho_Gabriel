#include "AudioPlayer.h"

// Arrays holding 8-bit samples to be played at 16kHz
// and their sizes (from samples.h)
//
//  const uint8_t  HAL[] = {...};
//  const uint32_t HALSize = sizeof(HAL) / sizeof(HAL[0]);
//  const uint8_t  Eurythmics[] = {...F};
//  const uint32_t EurythmicsSize = sizeof(Eurythmics) / sizeof(Eurythmics[0]);
//  const uint8_t  Sine1kHz[] = {...};
//  const uint32_t Sine1kHzSize = sizeof(Sine1kHz) / sizeof(Sine1kHz[0]);
//
#include "samples.h"


void setup() {
  DACSetup(16000, 4);                             // Set up DAC for 16kHz playback, 4x oversampling
}

void loop() {
      delay(200);
      playSample(Eurythmics, EurythmicsSize);
      delay(1500);
}
