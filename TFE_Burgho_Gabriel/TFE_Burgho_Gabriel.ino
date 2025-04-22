
//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

uint8_t sample[sample_size];

void setup() 
{
  pinMode(pin_MOSI, INPUT);
  pinMode(pin_MISO, OUTPUT);
  pinMode(pin_CS, INPUT);
  pinMode(pin_SCK, INPUT);

  pinMode(pin_SHUTDOWN, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  generateSineWave();
  setupDAC();
  setupTimer_DAC();
  digitalWrite(pin_SHUTDOWN, 0);  //désactive le shutdown
}
void MOSI_1(void)
{
  REG_PORT_OUT1 |= 0b100 0000 0000;
}
void MOSI_0(void)
{
  REG_PORT_OUT1 &= !0b100 0000 0000;
}
void SCK_1(void)
{
  REG_PORT_OUT1 |= 0b 1000 0000 0000;
}
void SCK_0(void)
{
  REG_PORT_OUT1 &= !0b 1000 0000 0000;
}
void MISO_1(void)
{
  REG_PORT_OUT0 |= 0b 1 0000 0000 0000;
}
void MISO_0(void)
{
  REG_PORT_OUT0 &= !0b 1 0000 0000 0000;
}
void CS_1(void)
{
  REG_PORT_OUT0 |= 0b 10 0000 0000 0000 0000;
}
void CS_0(void)
{
  REG_PORT_OUT0 &= !0b 10 0000 0000 0000 0000;
}
void mettre_pin_OUT0(int pin, bool state)
{
  REG_PORT_OUT0 |= (state << pin);
}
void mettre_pin_OUT1(int pin, bool state)
{
  REG_PORT_OUT1 |= (state << pin);
}
void loop() 
{
}