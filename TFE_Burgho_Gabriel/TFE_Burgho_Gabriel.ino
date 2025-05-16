//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

uint8_t buffer_in[250];
uint8_t buffer_out[250];
void setup() {
  Serial.begin(9600);
  analogWriteResolution(10);
  pinMode(pin_MOSI, INPUT);
  pinMode(pin_MISO, OUTPUT);
  pinMode(pin_CS, INPUT_PULLUP);
  pinMode(pin_SCK, INPUT);
  pinMode(pin_SHUTDOWN, OUTPUT);
  digitalWrite(pin_MISO, 0);
  digitalWrite(pin_SHUTDOWN, 0);  //désactive le shutdown*/
  //while(1) Serial.println("bloqué");
}

void loop() {
  if (!digitalRead(pin_CS)) {
    while (!digitalRead(pin_SCK));  //communication half duplex, le premier bit est pour le choix du mode, 1 si c'est pour parler et 0 pour écouter
    bool mode = digitalRead(pin_MOSI);
    while (digitalRead(pin_SCK));

    if (mode) {
      while (!digitalRead(pin_CS)) 
      {
        //test mode neopixel verte
        /*buffer_in[0] = analogRead(A1);
          for (int i = 0; i < 8; i++) 
          {
            while (!digitalRead(pin_SCK));
            digitalWrite(pin_MISO, bitRead(buffer_in[0] , i));
            while (digitalRead(pin_SCK));
          }
          */

      }
    } else {
      while (!digitalRead(pin_CS))
      {
        //test mode neopixel rouge
        /*
            for (int i = 0; i < 8; i++) {
      while (!digitalRead(pin_SCK));
      bitWrite(buffer_out[0], i, digitalRead(pin_MOSI));
      while (digitalRead(pin_SCK));
    }
    analogWrite(A0, buffer_out[0]);*/
      }


    }
  }

}