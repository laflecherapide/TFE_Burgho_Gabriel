#include "T-W_espnow.h"

void setup()
{
  initEspNow();
}
void loop() 
{
  for(int i = 0;i<sample_size;i++)
{
  Serial.println(sample[i]);
  delay(1);
}
}
