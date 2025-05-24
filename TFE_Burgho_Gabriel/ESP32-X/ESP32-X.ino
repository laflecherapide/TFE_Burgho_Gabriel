#include "T-W_espnow.h"

uint8_t buffer_parler[250];

void setup() 
{
  pinMode(pin_PUSH_TO_TALK, INPUT_PULLUP);
  pinMode(pin_TENSION, INPUT);
  pinMode(pin_BP_ALLUMAGE, INPUT);
  pinMode(pin_ENABLE_REGU, OUTPUT);
  pinMode(pin_MISO, INPUT);
  pinMode(pin_CS, OUTPUT);
  pinMode(pin_SCK, OUTPUT);
  pinMode(pin_MOSI, OUTPUT);
  digitalWrite(pin_ENABLE_REGU, 1);  // Active le régulateur

  Serial.begin(9600);
  init_display(display_adress);


  digitalWrite(pin_SCK, 0);
  digitalWrite(pin_MISO, 0);
  digitalWrite(pin_MOSI, 0);
  digitalWrite(pin_CS, 1);

  initEspNow();
}

void loop() 
{
  if (!digitalRead(pin_PUSH_TO_TALK))
  {//test envois
    digitalWrite(pin_CS,0);
    choix_du_mode(PARLER);
    for (int u = 0; u < 250; u++)
    {
      for (int i = 0; i < 8;i++)
    {
      digitalWrite(pin_SCK, 1);
      delayMicroseconds(2);
      bitWrite(buffer_parler[u], i, digitalRead(pin_MISO));
      delayMicroseconds(2);
      digitalWrite(pin_SCK,0);
      delayMicroseconds(2);
    }
    Serial.println(buffer_parler[u]);
    }
    digitalWrite(pin_CS, 1);
    esp_err_t result = esp_now_send(mac_O, buffer_parler, sizeof(buffer_parler));
    if (result == ESP_OK)
    {
      Serial.println("envois réussi");
    } else 
    {
      Serial.println("envois raté");
    }
  }
/*envois 
      digitalWrite(pin_MOSI, bitRead(send_data, i));
      delayMicroseconds(2);
      digitalWrite(pin_SCK, 1);
      delayMicroseconds(2);
      digitalWrite(pin_SCK,0);
      delayMicroseconds(2);
*/


  /*
    slave_data = 0;
    reception
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
  /*
  if (digitalRead(pin_BP_ALLUMAGE))
  {
    if (!digitalRead(pin_BP_ALLUMAGE)) digitalWrite(pin_ENABLE_REGU, 0);
  }

  bool charge = 0;
  float tension = mesure_tension();//j'en fais une variable pour que les comparaisons se fassent à un instant t, si la tension varie.
  if (tension >= 3.7 && tension  <= 4.2)
  {
    digitalWrite(pin_ENABLE_REGU,1);
    charge = 0;
  } else if (tension > 4.2) 
  {
    charge = 1;
  }
  if (tension <= 3.5) 
  {
    while (digitalRead(pin_BP_ALLUMAGE));
    digitalWrite(pin_ENABLE_REGU,0);
  }
  display.setCursor(0,0);
  afficharge(tension, charge);
  refresh();
  */
}
