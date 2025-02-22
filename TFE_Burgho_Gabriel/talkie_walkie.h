//****************LIBRAIRIE*****************
#define PIN_DAC A0
#define PIN_ADC A1
#define buffer_size 1024
#define TABLE_SIZE 1024 // Nombre d'échantillons
#define AMPLITUDE 512  // Demi-amplitude pour 10 bits (1024 niveaux)
#define OFFSET 512     // Offset pour centrer sur 0-1023
extern int buffer[buffer_size];