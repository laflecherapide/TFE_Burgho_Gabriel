#include "T-W_espnow.h"
#define CS_PIN D7
void setup()
{
  init_display(display_adress);
  pinMode(ENABLE_REGU,OUTPUT);
  digitalWrite(A0, 1);//active le enable du regu pour que l'ensemble du circuit soit alimenté en permanence pendant les tests
  initEspNow();

  SPI.begin();
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH); // Désactiver CS par défaut
}

void loop() 
{

  for(int i = 0;i<sample_size;i++)
{
  SPI.transfer(sample[i]);
  Serial.println(sample[i]);
  delay(10);
}
}