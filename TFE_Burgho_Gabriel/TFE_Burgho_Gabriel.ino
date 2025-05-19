//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

#include <Adafruit_NeoPixel.h>   // Librairie à inclure pour piloter facilement la NeoPixel

#define NEOPIXEL_PIN 8        // Remplacer les ... par le numéro de la pin utilisée pour la NeoPixel
                                 // A trouver dans le schéma de la Feather M0 express (annexe 1 du syllabus)
                                 // (tout comme la 13 est utilisée pour la LED rouge)

Adafruit_NeoPixel NeoPixel(1, NEOPIXEL_PIN, NEO_GRB); // Déclaration de l'objet NeoPixel
                                                      // 1er paramètre = nombre de LED
                                                      // 2ème paramètre = numéro de la pin
                                                      // 3ème paramètre = type de LED utilisée


void setup() {
    NeoPixel.begin();
  NeoPixel.setPixelColor(0, 0, 0, 0);
  NeoPixel.show();
  Serial.begin(9600);
  analogWriteResolution(8);
  analogReadResolution(8);
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
  /*
  if (!digitalRead(pin_CS))//esp_X
  {
    buffer_parler[0] = analogRead(A1);
      for (int i = 0; i < 8; i ++)
    {
      while (!digitalRead(pin_SCK));
      digitalWrite(pin_MISO, bitRead(buffer_parler[0] , i));
      while (digitalRead(pin_SCK));
    }
  }
*/
  if (!digitalRead(pin_CS))//esp_o
  {
    for (int i = 0; i < 8; i++) 
    {
      while (!digitalRead(pin_SCK));
      bitWrite(buffer_entendre[0], i, digitalRead(pin_MOSI));
      while (digitalRead(pin_SCK));
    }
    analogWrite(A0, buffer_entendre[0]);
  }


/*
  if (!digitalRead(pin_CS)) {
    while (!digitalRead(pin_SCK));  //communication half duplex, le premier bit est pour le choix du mode, 1 si c'est pour parler et 0 pour écouter
    bool mode = digitalRead(pin_MOSI);
    while (digitalRead(pin_SCK));

    if (mode) {
      while (!digitalRead(pin_CS)) 
      {
        NeoPixel.setPixelColor(0, 0, 120, 0);
  NeoPixel.show();//test mode neopixel verte
        buffer_in[0] = analogRead(A1);
          for (int i = 0; i < 8; i++) 
          {
            while (!digitalRead(pin_SCK));
            digitalWrite(pin_MISO, bitRead(buffer_in[0] , i));
            while (digitalRead(pin_SCK));
          }
          

      }
    } else {
      while (!digitalRead(pin_CS))
      {
        NeoPixel.setPixelColor(0, 120, 0, 0);
  NeoPixel.show();//test mode neopixel rouge
        
            for (int i = 0; i < 8; i++) {
      while (!digitalRead(pin_SCK));
      bitWrite(buffer_out[0], i, digitalRead(pin_MOSI));
      while (digitalRead(pin_SCK));
    }
    analogWrite(A0, buffer_out[0]);
      }


    }
  }
*/
}