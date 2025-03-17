//https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_now.html
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <math.h>

#define sample_size 250

extern uint8_t sample[sample_size];
extern const uint8_t mac_TX[];//header_pin
extern const uint8_t mac_RX[];//PCB
extern esp_now_peer_info_t peerInfo;
extern String success;

void initEspNow(void);
void OnDataRecv(const uint8_t *, const uint8_t *, int );
void generateSample(void);
void OnDataSent(const uint8_t *, esp_now_send_status_t );