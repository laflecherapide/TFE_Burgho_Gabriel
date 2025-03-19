#include "T-W_espnow.h"

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);

uint8_t sample[sample_size];

const byte display_adress = 0x3C;
const uint8_t mac_TX[] = {0x64, 0xE8, 0x33, 0x86, 0x57, 0x00};//header_pin
const uint8_t mac_RX[] = {0x54, 0x32, 0x04, 0x86, 0xDA, 0x1C};//PCB

esp_now_peer_info_t peerInfo;

String success;

void refresh(void)
{
  display.display();
  display.clearDisplay();
}

void init_display(const byte adress)
{
  display.begin(SSD1306_SWITCHCAPVCC, adress); // Address 0x3C for 128x32
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
}

void initEspNow(void) 
{
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();  // Éviter les conflits Wi-Fi

  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);                // Forcer le canal 1
  esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR);          // Activer le mode Long Range
  esp_wifi_set_max_tx_power(78);                                 // Maximiser la puissance TX
  esp_wifi_config_espnow_rate(WIFI_IF_STA, WIFI_PHY_RATE_1M_L);  // Optimiser pour longue portée

  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));  //callBack reception espnow
  // ✅ Vérification du mode Long Range activé
  uint8_t protocol = 0;
  esp_err_t err = esp_wifi_get_protocol(WIFI_IF_STA, &protocol);
  if (err == ESP_OK && (protocol & WIFI_PROTOCOL_LR)) 
  {
    Serial.println(F("✅ espNOW : Mode Long Range activé sur le récepteur"));
  } else 
  {
    Serial.println(F("❌ espNOW :  Mode Long Range NON activé sur le récepteur"));
  }
  // Register peer
  memcpy(peerInfo.peer_addr, mac_RX, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) 
  {
    Serial.println("Failed to add peer");
    return;
  }
}
// Callback when data is received
void OnDataRecv(const uint8_t *mac_TX, const uint8_t *incomingData, int len) 
{
  memcpy(sample, incomingData, sizeof(sample));
}
// Callback when data is sent
void OnDataSent(const uint8_t *mac_RX, esp_now_send_status_t status) 
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0) 
  {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Fail :(";
  }
}
void generateSample(void) {
    for (int i = 0; i < sample_size; i++) 
    {
      sample[i] = i;
    }
}