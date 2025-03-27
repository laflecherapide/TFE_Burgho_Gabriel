#include "T-W_espnow.h"
#include <SPI.h>

#define CS_PIN D7  // GPIO20
bool data_SPI = 0;

void setup()
{
  /*init_display(display_adress);
  pinMode(ENABLE_REGU, OUTPUT);
  digitalWrite(A0, 1); // Active le régulateur
  initEspNow();
  generateSample();
*/
  Serial.begin(9600);
  SPI.begin(8, 9, 10, CS_PIN); // SCK = GPIO8, MISO = GPIO9, MOSI = GPIO10, SS = GPIO20
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH); // Désactiver CS par défaut
}

void loop() 
{
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS_PIN, LOW);  // Activer l'esclave

  for (int i=0;i<sample_size;i++ )
  {
    data_SPI = !data_SPI;
      SPI.transfer(data_SPI);
      Serial.println(data_SPI);
  }

  digitalWrite(CS_PIN, HIGH); // Désélectionner l'esclave
  SPI.endTransaction();

}
