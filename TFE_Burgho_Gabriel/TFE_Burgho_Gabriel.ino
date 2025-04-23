//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

uint8_t sample[sample_size];
uint8_t slaveData = 0;
uint8_t sendData = 0;

void setup() 
{
  pinMode(pin_MOSI, INPUT);
  pinMode(pin_MISO, OUTPUT);
  pinMode(pin_CS, INPUT);
  pinMode(pin_SCK, INPUT);

  pinMode(pin_SHUTDOWN, OUTPUT);
  Serial.begin(9600);
  generateSineWave();
  setupDAC();
  setupTimer_DAC();
  digitalWrite(pin_SHUTDOWN, 0);  //désactive le shutdown
}

void loop() 
{
  fast_digitalWrite(pin_CS, LOW);
  slaveData = bitBangData(sendData);
  fast_digitalWrite(pin_CS, HIGH);
}