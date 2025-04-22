#include "T-W_espnow.h"

void setup()
{
  init_display(display_adress);
  pinMode(pin_TENSION, INPUT);
  pinMode(pin_BP_ALLUMAGE, INPUT);
  pinMode(pin_ENABLE_REGU, OUTPUT);

  pinMode(pin_MISO, INPUT);
  pinMode(pin_CS, OUTPUT);
  pinMode(pin_SCK, OUTPUT);
  pinMode(pin_MOSI, OUTPUT);
  digitalWrite(pin_CS, HIGH);

  digitalWrite(pin_ENABLE_REGU, 1); // Active le régulateur
  initEspNow();
  generateSample();
  Serial.begin(9600);
}

byte bitBangData(byte _send)  // This function transmit the data via bitbanging
{
  byte _receive = 0;

  for(int i=0; i<8; i++)  // 8 bits in a byte
  {
    set_pin(pin_MOSI, bitRead(_send, i));    // Set MOSI
    set_pin(pin_SCK, HIGH);                  // SCK high
    bitWrite(_receive, i, read_pin(pin_MISO)); // Capture MISO
    set_pin(pin_SCK, LOW);                   // SCK low
  } 
  return _receive;        // Return the received data
}

void loop() 
{
  digitalWrite(pin_CS, LOW);
  slaveData = bitBangData(sendData); // data transmission
  digitalWrite(pin_CS, HIGH);

  bool charge = 0;
  float tension = mesure_tension();//j'en fais une variable pour que les comparaisons se fassent à un instant t, si la tension varie.
  if (tension >= 3.7 && tension  <= 4.2)
  {
    digitalWrite(ENABLE_REGU,1);
    charge = 0;
  } else if (tension > 4.2) 
  {
    charge = 1;
  }
  if (tension <= 3.6) 
  {
    digitalWrite(ENABLE_REGU,0);
  }
  afficharge(tension, charge);
  refresh();
}
