//****************LIBRAIRIE*****************
#include "talkie_walkie.h"
#include <fast_samd21_tc3.h>//0.3
#include <fast_samd21_tc4.h>//0.3

void TC3_Handler(void)//doit être dans le ino sinon erreur de librairie
{//entree ADC remplir buffer
  static volatile uint8_t currentIndex = 0;
  buffer_parler[currentIndex] = analogRead(A1);
  currentIndex++;
  TC3->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
  if (currentIndex >= sample_size) 
  {//revenir à la valeur 0 du tableau
    currentIndex = 0;
    fast_samd21_tc3_disable();
  }
}

void TC4_Handler(void)//doit être dans le ino sinon erreur de librairie
{//sortie DAC vider buffer
  static volatile uint8_t currentIndex = 0;
  //analogWriteDAC(buffer_entendre[currentIndex]);
  currentIndex++;
  TC4->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
  if (currentIndex >= sample_size) 
  {//vider un buffer
    currentIndex = 0;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(pin_MOSI, INPUT);
  pinMode(pin_MISO, OUTPUT);
  pinMode(pin_CS, INPUT_PULLUP);
  pinMode(pin_SCK, INPUT);
  pinMode(pin_SHUTDOWN, OUTPUT);

  digitalWrite(pin_MISO, 0);
  digitalWrite(pin_SHUTDOWN, 1);  //active le shutdown ampli AB

  fast_samd21_tc3_configure(10000); //us
  fast_samd21_tc4_configure(10000);//us
  fast_samd21_tc3_disable();
  fast_samd21_tc4_disable();
}


void envois_spi(void)
{
  static volatile uint8_t data_bit = 0;
  static volatile uint16_t indexe_sortie_SPI;
  digitalWrite(pin_MISO, bitRead(buffer_parler[indexe_sortie_SPI] , data_bit));
  wait_cycles(10000);//8MHz 1.25ms
  data_bit, indexe_sortie_SPI ++;
  if (indexe_sortie_SPI >= sample_size) indexe_sortie_SPI = 0;
  if (digitalRead(pin_CS)) detachInterrupt(pin_SCK);
}

void reception_spi(void)
{
  static volatile uint8_t data_bit = 0;
  static volatile uint16_t indexe_entree_SPI;
  bitWrite(buffer_entendre[indexe_entree_SPI], data_bit, digitalRead(pin_MOSI));
  wait_cycles(10000);//8MHz 1.25ms
  data_bit, indexe_entree_SPI ++;
  if (indexe_entree_SPI >= sample_size) indexe_entree_SPI = 0;
  if (digitalRead(pin_CS)) detachInterrupt(pin_SCK);
}

void desactive_TC(void)
{
  fast_samd21_tc3_disable();
  fast_samd21_tc4_disable();
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

  //************communication half duplex avec choix du mode***********
  if (!digitalRead(pin_CS)) 
    {
      while (!digitalRead(pin_SCK));  //communication half duplex, le premier bit est pour le choix du mode, 1 si c'est pour parler et 0 pour écouter
      bool mode = digitalRead(pin_MOSI);
      while (digitalRead(pin_SCK));
      if (!digitalRead(pin_CS) && mode) attachInterrupt(pin_SCK, envois_spi, RISING);
      if (!digitalRead(pin_CS) && !mode) attachInterrupt(pin_SCK, reception_spi, RISING);
      if (mode)
        {//active TC ADC
          fast_samd21_tc3_start();
          attachInterrupt(pin_CS, desactive_TC, RISING);
        } else {
          //active TC DAC
          fast_samd21_tc4_start();
          attachInterrupt(pin_CS, desactive_TC, RISING);
          }
  }

}