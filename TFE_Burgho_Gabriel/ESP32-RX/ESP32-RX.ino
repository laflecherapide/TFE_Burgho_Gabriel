#include "T-W_espnow.h"

void setup()
{
  init_display(display_adress);
  pinMode(pin_tension, INPUT);
  pinMode(BP_to_ON, INPUT);
  pinMode(ENABLE_REGU, OUTPUT);
  digitalWrite(A0, 1); // Active le régulateur
  initEspNow();
  generateSample();
  Serial.begin(9600);
}

void loop() 
{
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
