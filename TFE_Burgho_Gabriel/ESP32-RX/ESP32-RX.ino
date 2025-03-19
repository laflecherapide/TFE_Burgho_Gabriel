#include "T-W_espnow.h"

void setup()
{
  init_display(display_adress);
  pinMode(ENABLE_REGU,OUTPUT);
  digitalWrite(A0, 1);//active le enable du regu pour que l'ensemble du circuit soit alimenté en permanence pendant les tests
  initEspNow();
}
void loop() 
{
  for(int i = 0;i<sample_size;i++)
{
  Serial.println(sample[i]);
  delay(10);
}
}