//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

uint8_t sample[sample_size];
uint8_t masterData = 0;
uint8_t sendData = 32;
    byte data = 0;
void setup() 
{
  Serial.begin(9600);
  pinMode(pin_MOSI, INPUT);
  pinMode(pin_MISO, OUTPUT);
  pinMode(pin_CS, INPUT_PULLUP);
  pinMode(pin_SCK, INPUT);
  pinMode(pin_SHUTDOWN, OUTPUT);
  /*generateSineWave();
  setupDAC();
  setupTimer_DAC();*/
  digitalWrite(pin_SHUTDOWN, 0);  //désactive le shutdown
  //while(1) Serial.println("bloqué");
}

void loop() 
{
  if (!digitalRead(pin_CS))
  {
    for (int i = 0; i < 8; i++)
    {
      if (digitalRead(pin_SCK))
      {
        data &= (digitalRead(pin_MOSI) << i);
      }
    }
  }
  Serial.println(data);
  /*if (!fast_digitalRead(pin_CS))
  {
      masterData = bitBangData(sendData);
  }
  Serial.println(masterData);*/
}