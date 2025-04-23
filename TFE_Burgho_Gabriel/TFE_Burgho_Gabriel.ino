
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

static inline void wait_cycles(uint32_t n) {
    while(n--) {
        __asm__ volatile ("nop");
    }
}

byte bitBangData(byte _send)  // This function transmit the data via bitbanging
{
  byte _receive = 0;

  for(int i=0; i<8; i++)  // 8 bits in a byte
  {
    fast_digitalWrite(pin_MOSI, bitRead(_send, i));    // Set MOSI
    fast_digitalWrite(pin_SCK, HIGH);                  // SCK high
    bitWrite(_receive, i, fast_digitalRead(pin_MISO)); // Capture MISO
    fast_digitalWrite(pin_SCK, LOW);                   // SCK low
  } 
  return _receive;        // Return the received data
}

int fast_digitalRead( uint32_t ulPin )
{
  if ( (PORT->Group[g_APinDescription[ulPin].ulPort].IN.reg & (1ul << g_APinDescription[ulPin].ulPin)) != 0 )
  {
    return HIGH ;
  }

  return LOW ;
}
void fast_digitalWrite( uint32_t ulPin, uint32_t ulVal )
{
  EPortType port = g_APinDescription[ulPin].ulPort;
  uint32_t pin = g_APinDescription[ulPin].ulPin;
  uint32_t pinMask = (1ul << pin);
  switch ( ulVal )
  {
    case LOW:
      PORT->Group[port].OUTCLR.reg = pinMask;
    break ;

    default:
      PORT->Group[port].OUTSET.reg = pinMask;
    break ;
  }
}

void loop() 
{
  fast_digitalWrite(pin_CS, LOW);
  slaveData = bitBangData(sendData);
  fast_digitalWrite(pin_CS, HIGH);
}