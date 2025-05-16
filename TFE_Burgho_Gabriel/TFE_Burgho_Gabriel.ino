//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

uint8_t sample[sample_size];
uint8_t sample_sent[sample_size];
void setup() {
  Serial.begin(9600);
  pinMode(pin_MOSI, INPUT);
  pinMode(pin_MISO, OUTPUT);
  pinMode(pin_CS, INPUT_PULLUP);
  pinMode(pin_SCK, INPUT);
  pinMode(pin_SHUTDOWN, OUTPUT);
  digitalWrite(pin_MISO, 0);
  setupADC();
  /*generateSineWave();
  setupDAC();
  setupTimer_DAC();
  digitalWrite(pin_SHUTDOWN, 0);  //désactive le shutdown*/
  //while(1) Serial.println("bloqué");
}

void loop() {
  if (!digitalRead(pin_CS)) {
    while (!digitalRead(pin_SCK))
      ;  //communication half duplex, le premier bit est pour le choix du mode, 1 si c'est pour parler et 0 pour écouter
    bool mode = digitalRead(pin_MOSI);
    while (digitalRead(pin_SCK))
      ;

    if (mode) {
      while (digitalRead(pin_CS)) 
      {
        for (int j = 0; j<250; j++)
        {
          sample_sent[j] = ADC_mesurement();
        }
        for (int u = 0; u < 250; u++) 
        {
          for (int i = 0; i < 8; i++) 
          {
            while (!digitalRead(pin_SCK));
            digitalWrite(pin_MISO, bitRead(sample_sent[u], i));
            while (digitalRead(pin_SCK));
          }
          sample_sent[u] = 0;
        }
      }
    } else {
    }
  }
  if (!digitalRead(pin_CS)) {
    for (int i = 0 i < 8 i++) {
      while (!digitalRead(pin_SCK))
        ;
      bitWrite(master_data, i, digitalRead(pin_MOSI));
      while (digitalRead(pin_SCK))
        ;
    }
  }


  /*if (!digitalRead(pin_CS))
  {
    for (int i = 0; i < 8; i++)
    {
      while(!digitalRead(pin_SCK));
      digitalWrite(pin_MISO, bitRead(data_sent, i));
      while(digitalRead(pin_SCK));

    }
  }*/
}