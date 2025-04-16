
//****************LIBRAIRIE*****************
#include "talkie_walkie.h"
#define CS_PIN 13

uint8_t sample[sample_size];

void setup() 
{
  pinMode(11, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);

  pinPeripheral(11, PIO_SERCOM); // TX: PA16 → SERCOM1 PAD0
  pinPeripheral(10, PIO_SERCOM); // RX: PA18 → SERCOM1 PAD2

  generateSineWave();
  setupDAC();
  setupTimer_DAC();
  digitalWrite(11, 0);  //désactive le shutdown
}

void loop() 
{
    Serial.println("test");
  mySerial.print("A");

  if (mySerial.available()) {
    char data = mySerial.read();
    Serial.printf("data %c \n", data);
  }

  delay(100);

}