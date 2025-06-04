//SPI bit banging : https://circuitdigest.com/article/introduction-to-bit-banging-spi-communication-in-arduino-via-bit-banging

//****************LIBRAIRIE*****************
#include <stdint.h>
#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <fast_samd21_tc3.h>//0.3
#include <fast_samd21_tc4.h>//0.3

//doc : https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports?view=all#creating-a-new-serial

#define AMPLITUDE 512
#define OFFSET 512
#define WIRE Wire
#define sample_size 250
#define pin_MOSI 29
#define pin_MISO 28
#define pin_CS 13
#define pin_SCK 30
#define pin_SHUTDOWN 11

extern uint8_t buffer_parler[];
extern uint8_t buffer_entendre[];

void setupTimer_DAC(void);
void generatesample(void);
void setupDAC(void);
void setupADC(void);
void TC3_Handler(void);
uint8_t ADC_mesurement(void);
static inline void wait_cycles(uint32_t);
int fast_digitalRead( uint32_t );
void fast_digitalWrite( uint32_t, uint32_t);
inline void analogWriteDAC(uint16_t);