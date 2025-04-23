#include "T-W_espnow.h"

uint8_t sendData = 2;
uint8_t slaveData = 0;
void setup()
{
  Serial.begin(9600);
  while(!Serial);
  //init_display(display_adress);
  //while(1) Serial.println("bloqué");
  pinMode(pin_TENSION, INPUT);
  pinMode(pin_BP_ALLUMAGE, INPUT);
  pinMode(pin_ENABLE_REGU, OUTPUT);
  pinMode(pin_MISO, INPUT);
  pinMode(pin_CS, OUTPUT);
  pinMode(pin_SCK, OUTPUT);
  pinMode(pin_MOSI, OUTPUT);
  digitalWrite(pin_CS, HIGH);
  digitalWrite(pin_ENABLE_REGU, 1); // Active le régulateur
  //initEspNow();
  //generateSample();
}

byte bitBangData(byte _send)  // This function transmit the data via bitbanging
{
  byte _receive = 0;

  for(int i=0; i<8; i++)  // 8 bits in a byte
  {
    set_pin(pin_MOSI, bitRead(_send, i));    // Set MOSI
    set_pin(pin_SCK, HIGH);                  // SCK high
    delayMicroseconds(10);
    bitWrite(_receive, i, read_pin(pin_MISO)); // Capture MISO
    set_pin(pin_SCK, LOW);                   // SCK low
  } 
  return _receive;        // Return the received data
}

void Serial_gab_Data(byte data)
{
  digitalWrite(pin_CS, 0);
  for (int i = 0; i < 8; i++)
  {
      if (data & 1<<i) 
      {
        digitalWrite(pin_MOSI, 1);
      } else {
        digitalWrite(pin_MOSI, 0);
      }
      set_pin(pin_SCK, 1);
      delay(1);
      set_pin(pin_SCK, 0);
      delay(1);
  }
  digitalWrite(pin_CS, 1);
}

void loop() 
{
  Serial_gab_Data(sendData);
  Serial.println("data envoyé");
  /*digitalWrite(pin_CS, LOW);
  slaveData = bitBangData(sendData); // data transmission
  digitalWrite(pin_CS, HIGH);
  Serial.println(slaveData);*/
  /*bool charge = 0;
  float tension = mesure_tension();//j'en fais une variable pour que les comparaisons se fassent à un instant t, si la tension varie.
  if (tension >= 3.7 && tension  <= 4.2)
  {
    digitalWrite(pin_ENABLE_REGU,1);
    charge = 0;
  } else if (tension > 4.2) 
  {
    charge = 1;
  }
  if (tension <= 3.6) 
  {
    digitalWrite(pin_ENABLE_REGU,0);
  }
  afficharge(tension, charge);
  refresh();*/
}
