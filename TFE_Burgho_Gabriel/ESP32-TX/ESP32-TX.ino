#include "T-W_espnow.h"

void setup() 
{
  init_display(display_adress);
  generateSample();
  initEspNow();
}

void loop() 
{
  delay(500);//pour limiter les envois car l'ESP32 chauffe un peu
  esp_err_t result = esp_now_send(mac_RX, (uint8_t *)&sample, sizeof(sample));
  if (result == ESP_OK) 
  {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
}
