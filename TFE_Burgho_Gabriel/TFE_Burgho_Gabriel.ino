
//****************LIBRAIRIE*****************
#include "talkie_walkie.h"
#define CS_PIN 13

uint8_t sample[sample_size];

void setup() 
{
  pinMode(CS_PIN, INPUT_PULLUP);

  pinMode(11, OUTPUT);
  Serial.begin(115200);
  generateSineWave();
  setupDAC();
  setupTimer_DAC();
  digitalWrite(11, 0);  //désactive le shutdown
}

void loop() 
{

}