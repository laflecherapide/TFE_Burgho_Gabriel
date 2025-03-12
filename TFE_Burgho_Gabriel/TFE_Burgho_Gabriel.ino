
//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

void setup() {
    Serial.begin(115200);
    generateSineWave();
    setupDAC();
    setupTimer();
    pinMode(11,OUTPUT);
}

void loop() {
  digitalWrite(11,  0);//désactive le shutdown
}