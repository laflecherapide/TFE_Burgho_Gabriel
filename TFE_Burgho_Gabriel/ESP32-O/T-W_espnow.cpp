#include "esp32-hal.h"
#include "T-W_espnow.h"

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);

uint8_t sample[sample_size];

uint8_t buffer_entendre[250];

const byte display_adress = 0x3C;
const uint8_t mac_X[] = {0x54, 0x32, 0x04, 0x86, 0xE4, 0x0C};
const uint8_t mac_O[] = {0x18, 0x8B, 0x0E, 0x93, 0x7B, 0x94};

esp_now_peer_info_t peerInfo;

String success;

void set_pin(int pin,bool val)
{
    gpio_set_level((gpio_num_t)pin, val);
}

int read_pin(int pin)
{
  return gpio_get_level((gpio_num_t)pin);
}

float mesure_tension(void)
{
uint32_t Vbatt = 0;
  for(int i = 0; i < 16; i++) {
    Vbatt = Vbatt + analogReadMilliVolts(pin_TENSION); // ADC with correction   
  }
  float tension = 2 * Vbatt / 16 / 1000.0;
  return tension+0.25;//seuil de diode
}

void afficharge(float var1, bool var2)
{
  display.print("V = ");
  display.println(var1);
  if (var2 == 1)
  {
    display.print("charge");
  } else 
  {
    display.print("pas charge");
  }
}

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
  memcpy(peerInfo.peer_addr, mac_X, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) 
  {
    Serial.println("Failed to add peer");
    return;
  }
}

void choix_du_mode(bool mode)
{
    digitalWrite(pin_MOSI, mode);
    delayMicroseconds(2);
    digitalWrite(pin_SCK, 1);
    delayMicroseconds(2);
    digitalWrite(pin_SCK, 0);
    delayMicroseconds(2);
}

// Callback when data is received
void OnDataRecv(const uint8_t *mac_X, const uint8_t *incomingData, int len) 
{
  memcpy(buffer_entendre, incomingData, sizeof(buffer_entendre));
  digitalWrite(pin_CS, 0);
  choix_du_mode(ENTENDRE);
  for (int u = 0; u < 250; u++)
  {
    Serial.println(buffer_entendre[u]);
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(pin_MOSI, bitRead(buffer_entendre[u], i));
      delayMicroseconds(2);
      digitalWrite(pin_SCK, 1);
      delayMicroseconds(2);
      digitalWrite(pin_SCK,0);
      delayMicroseconds(2);
    } 
  }
  digitalWrite(pin_CS, 1);
}
// Callback when data is sent
void OnDataSent(const uint8_t *mac_O, esp_now_send_status_t status) 
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