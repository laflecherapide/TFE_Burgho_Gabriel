//SPI bit banging : https://circuitdigest.com/article/introduction-to-bit-banging-spi-communication-in-arduino-via-bit-banging

//****************LIBRAIRIE*****************
#include <stdint.h>
#include <Arduino.h>
#include <math.h>
#include <Wire.h>



//doc : https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports?view=all#creating-a-new-serial

#define WIRE Wire
#define sample_size 10000
#define pin_MOSI 29
#define pin_MISO 28
#define pin_CS 13
#define pin_SCK 30
#define pin_SHUTDOWN 11

//inline doit être dans le .h ou le .ino, dans le cpp il n'est pas reconnu : eviter "unresolved external" source : https://isocpp.org/wiki/faq/inline-functions#
static inline void wait_cycles(uint32_t n) {//delay bas niveau chatgpt sauf commentaire
//static signifie que c'est stocké en dehors de la pile, donc 
//inline signifie que le compilateur mette le bout de code à chaque fois qu'on appel la fonction au lieu d'appelé la fonction
    while(n--) {
        __asm__ volatile ("nop"); 
        //__asm__ signifie que c'est de l'assembleur
        //volatile signifie que elle est accessible
        //"nop" signifie pour le processeur de ne rien faire
    }
}


extern uint8_t buffer_parler[];
extern uint8_t buffer_entendre[];

void setupTimer_DAC(void);
void generatesample(void);
void setupDAC(void);
void setupADC(void);
void TC3_Handler(void);
uint8_t ADC_mesurement(void);
int fast_digitalRead( uint32_t );
void fast_digitalWrite( uint32_t, uint32_t);
