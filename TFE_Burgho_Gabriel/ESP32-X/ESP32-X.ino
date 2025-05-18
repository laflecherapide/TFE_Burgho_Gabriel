#include "T-W_espnow.h"
bool mode = 0;

void setup() {
  Serial.begin(9600);
  init_display(display_adress);
  pinMode(pin_PUSH_TO_TALK, INPUT_PULLUP);
  pinMode(pin_TENSION, INPUT);
  pinMode(pin_BP_ALLUMAGE, INPUT);
  pinMode(pin_ENABLE_REGU, OUTPUT);
  pinMode(pin_MISO, INPUT);
  pinMode(pin_CS, OUTPUT);
  pinMode(pin_SCK, OUTPUT);
  pinMode(pin_MOSI, OUTPUT);

  digitalWrite(pin_SCK, 0);
  digitalWrite(pin_MISO, 0);
  digitalWrite(pin_MOSI, 0);
  digitalWrite(pin_CS, 1);
  delay(500);
  //initEspNow();
    digitalWrite(pin_ENABLE_REGU, 1);  // Active le régulateur
    //while (digitalRead(pin_BP_ALLUMAGE));
}

void loop() 
{
  if (!digitalRead(pin_PUSH_TO_TALK))
  {
    digitalWrite(pin_CS,0);
    digitalWrite(pin_MOSI, 1);
    delayMicroseconds(2);
    digitalWrite(pin_SCK, 1);
    delayMicroseconds(2);
    digitalWrite(pin_SCK, 0);
    delayMicroseconds(2);
  }
  /*if (!digitalRead(pin_PUSH_TO_TALK))
  {
    digitalWrite(pin_CS,0);
    for (int i = 0; i < 8;i++)
    {
      digitalWrite(pin_MOSI, bitRead(send_data, i));
      delayMicroseconds(2);
      digitalWrite(pin_SCK, 1);
      delayMicroseconds(2);
      digitalWrite(pin_SCK,0);
      delayMicroseconds(2);
    }
    digitalWrite(pin_CS, 1);
  }*/



  /*
    slave_data = 0;
    digitalWrite(pin_CS,0);
    for (int i = 0;i<8;i++)
    {
      digitalWrite(pin_SCK, 1);
      delayMicroseconds(2);
      bitWrite(slave_data, i, digitalRead(pin_MISO));
      delayMicroseconds(2);
      digitalWrite(pin_SCK,0);
      delayMicroseconds(2);
    }
    Serial.println(slave_data);
    digitalWrite(pin_CS, 1);
  */
    
  /*bool charge = 0;
  float tension = mesure_tension();//j'en fais une variable pour que les comparaisons se fassent à un instant t, si la tension varie.
  if (tension >= 3.7 && tension  <= 4.2)
  {
    digitalWrite(pin_ENABLE_REGU,1);
    charge = 0;
    //if (digitalRead(pin_BP_ALLUMAGE)) digitalWrite(pin_ENABLE_REGU, 0);
  } else if (tension > 4.2) 
  {
    charge = 1;
  }
  if (tension <= 3.5) 
  {
    digitalWrite(pin_ENABLE_REGU,0);
  }
  display.setCursor(0,0);
  afficharge(tension, charge);
  refresh();*/
}
