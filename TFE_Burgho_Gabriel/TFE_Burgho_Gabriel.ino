//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

void setup() {
  analogWriteResolution(8);
  analogReadResolution(8);

  pinMode(pin_MOSI, INPUT);
  pinMode(pin_MISO, OUTPUT);
  pinMode(pin_CS, INPUT_PULLUP);
  pinMode(pin_SCK, INPUT);
  pinMode(pin_SHUTDOWN, OUTPUT);

  digitalWrite(pin_MISO, 0);
  digitalWrite(pin_SHUTDOWN, 0);  //active le shutdown
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
    digitalWrite(pin_SHUTDOWN, 0);
    while (!digitalRead(pin_SCK));  //communication half duplex, le premier bit est pour le choix du mode, 1 si c'est pour parler et 0 pour écouter
    bool mode = digitalRead(pin_MOSI);
    while (digitalRead(pin_SCK));

    switch (mode) 
    {
      case 1:
        buffer_parler[0] = analogRead(A1);
        for (int i = 0; i < 8; i ++)
          {
            while (!digitalRead(pin_SCK));
            digitalWrite(pin_MISO, bitRead(buffer_parler[0] , i));
            while (digitalRead(pin_SCK));
          }
        if (digitalRead(pin_CS)) break;
      case 0:
        digitalWrite(pin_SHUTDOWN, 1);
        for (int i = 0; i < 8; i++) 
          {
            while (!digitalRead(pin_SCK));
            bitWrite(buffer_entendre[0], i, digitalRead(pin_MOSI));
            while (digitalRead(pin_SCK));
          }
      
        analogWrite(A0, buffer_entendre[0]);
        if (digitalRead(pin_CS)) break;
    } 
  }
}