
//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

void setup() {
  pinMode(11, OUTPUT);
  Serial.begin(115200);
  generateSineWave();
  setupDAC();
  setupTimer();
  digitalWrite(11, 0);  //désactive le shutdown
}

void loop() {
}