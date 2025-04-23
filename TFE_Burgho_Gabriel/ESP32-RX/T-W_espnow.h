//esp now : https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_now.html
//SPI bit banging : https://circuitdigest.com/article/introduction-to-bit-banging-spi-communication-in-arduino-via-bit-banging
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WIRE Wire
#define pin_ENABLE_REGU D3
#define sample_size 250
#define pin_TENSION D0
#define pin_BP_ALLUMAGE D2
#define pin_MOSI D10
#define pin_MISO D9
#define pin_SCK D8
#define pin_CS D7


extern uint8_t sample[];
extern const uint8_t mac_TX[];//header_pin
extern const uint8_t mac_RX[];//PCB
extern esp_now_peer_info_t peerInfo;
extern String success;
extern const byte display_adress;

extern Adafruit_SSD1306 display;

void init_display(const byte);
void initEspNow(void);
void OnDataRecv(const uint8_t *, const uint8_t *, int );
void generateSample(void);
void OnDataSent(const uint8_t *, esp_now_send_status_t );
void refresh(void);
void afficharge(float,bool);
float mesure_tension(void);
void set_pin(int,bool);
int read_pin(int);