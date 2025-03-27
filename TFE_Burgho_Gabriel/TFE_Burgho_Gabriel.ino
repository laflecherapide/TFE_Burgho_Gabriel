
//****************LIBRAIRIE*****************
#include "talkie_walkie.h"
#define CS_PIN 13
volatile uint8_t dataBuffer[sample_size];
volatile uint16_t dataIndex = 0;
volatile bool dataReceived = false;
uint8_t sample[sample_size];

void setup() {
    pinMode(CS_PIN, INPUT);

    //J'aime le poulet et les lentilles

    // Configuration de la Feather en mode esclave SPI
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.attachInterrupt(); // Activer l'interruption SPI

  pinMode(11, OUTPUT);
  Serial.begin(115200);
  //generateSineWave();
  setupDAC();
  setupTimer();
  digitalWrite(11, 0);  //désactive le shutdown
}

void loop() {
    if (dataReceived) 
  {
        for (int i = 0; i < sample_size; i++) 
        {
          dataBuffer[i] = sample[i];
        }
        dataReceived = false;
    }
}

// Interruption SPI déclenchée à chaque octet reçu
void SERCOM4_Handler() {
    if (SERCOM4->SPI.INTFLAG.bit.RXC) { // Vérifier si un octet est reçu
        uint8_t receivedByte = SERCOM4->SPI.DATA.reg;
        if (dataIndex < sample_size) {
            dataBuffer[dataIndex++] = receivedByte;
        }
        if (dataIndex >= sample_size) {
            dataReceived = true;
            dataIndex = 0; // Réinitialisation pour le prochain transfert
        }
    }
}