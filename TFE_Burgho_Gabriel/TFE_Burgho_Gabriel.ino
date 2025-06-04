//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

volatile byte indexe_entree_SPI = 0, indexe_sortie_SPI = 0;

void setup() {
  setupDAC();

  pinMode(pin_MOSI, INPUT);
  pinMode(pin_MISO, OUTPUT);
  pinMode(pin_CS, INPUT_PULLUP);
  pinMode(pin_SCK, INPUT);
  pinMode(pin_SHUTDOWN, OUTPUT);

  digitalWrite(pin_MISO, 0);
  digitalWrite(pin_SHUTDOWN, 1);  //active le shutdown ampli AB

  fast_samd21_tc4_configure(100);  //µs
  fast_samd21_tc3_configure(100);  //µs
}

void loop() {
  //*********communication simplex pour les tests**********************
  /*
  if (!digitalRead(pin_CS))//pcb_X
  {
    buffer_parler[0] = analogRead(A1);
      for (int i = 0; i < 8; i ++)
    {
      while (!digitalRead(pin_SCK));
      digitalWrite(pin_MISO, bitRead(buffer_parler[0] , i));
      while (digitalRead(pin_SCK));
    }
  }*/

  /*
  if (!digitalRead(pin_CS))  //pcb_O
  {
    digitalWrite(pin_SHUTDOWN, 1);  //désactive le shutdown
    for (int i = 0; i < 8; i++) {
      while (!digitalRead(pin_SCK))
        ;
      bitWrite(buffer_entendre[0], i, digitalRead(pin_MOSI));
      while (digitalRead(pin_SCK))
        ;
    }
    analogWrite(A0, buffer_entendre[0]);
    digitalWrite(pin_SHUTDOWN, 0);  //active le shutdown
  }
*/

  //**************communication half duplex avec choix du mode***********
  if (!digitalRead(pin_CS)) 
    {
      while (!digitalRead(pin_SCK));  //communication half duplex, le premier bit est pour le choix du mode, 1 si c'est pour parler et 0 pour écouter
      bool mode = digitalRead(pin_MOSI);
      while (digitalRead(pin_SCK));
      if (mode)
        {//active TC ADC
          fast_samd21_tc3_start();
          for (int i = 0; i < 8; i++) 
            {
              while (!digitalRead(pin_SCK));
              digitalWrite(pin_MISO, bitRead(buffer_parler[indexe_sortie_SPI],i));
              while (digitalRead(pin_SCK));
            }
          indexe_sortie_SPI++;
          if (indexe_sortie_SPI >= sample_size) indexe_sortie_SPI = 0;
        } else {
          for (int u = 0; u < 250; u++) 
            {
              for (int i = 0; i < 8; i++) 
                {
                  while (!digitalRead(pin_SCK));
                  bitWrite(buffer_entendre[u], i, digitalRead(pin_MOSI));
                  while (digitalRead(pin_SCK));
                }
            }
          //active TC DAC
          fast_samd21_tc4_start();
                }
  }
}