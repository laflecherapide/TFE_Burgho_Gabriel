#include "samd21/include/samd21g18a.h"
#include <stdint.h>
//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

static inline void wait_cycles(uint32_t n) {
    while(n--) {
        __asm__ volatile ("nop");
    }
}

void fast_digitalWrite( uint32_t ulPin, uint32_t ulVal )
{
  EPortType port = g_APinDescription[ulPin].ulPort;
  uint32_t pin = g_APinDescription[ulPin].ulPin;
  uint32_t pinMask = (1ul << pin);
  switch ( ulVal )
  {
    case LOW:
      PORT->Group[port].OUTCLR.reg = pinMask;
    break ;

    default:
      PORT->Group[port].OUTSET.reg = pinMask;
    break ;
  }
}

int fast_digitalRead( uint32_t ulPin )
{
  if ( (PORT->Group[g_APinDescription[ulPin].ulPort].IN.reg & (1ul << g_APinDescription[ulPin].ulPin)) != 0 )
  {
    return HIGH ;
  }

  return LOW ;
}

byte bitBangData(byte _send)  // This function transmit the data via bitbanging
{
  byte _receive = 0;

  for(int i=0; i<8; i++)  // 8 bits in a byte
  {
    fast_digitalWrite(pin_MOSI, bitRead(_send, i));    // Set MOSI
    fast_digitalWrite(pin_SCK, HIGH);                  // SCK high
    bitWrite(_receive, i, fast_digitalRead(pin_MISO)); // Capture MISO
    fast_digitalWrite(pin_SCK, LOW);                   // SCK low
  } 
  return _receive;        // Return the received data
}

uint16_t sineTable[sample_size];  //tableau de int non signé (>1) de 16 bits
void setupDAC(void) {
  DAC->CTRLA.bit.ENABLE = 0;  //DAC est un pointer et on accede au membre CTRLA.bit.ENABLE grace à l'opérateur "->" et on le met à 0 ce qui désactive le DAC ce qui est nécessaire pour le configurer. CTRLA est une struct bit est une sous struct et ENABLE est un membre de la sous struct bit.
  while (DAC->STATUS.bit.SYNCBUSY)
    ;

  DAC->CTRLB.reg = DAC_CTRLB_EOEN | DAC_CTRLB_REFSEL(1);  //on choisit la reference du comparateur source Atmel | SMART SAM D21 Datasheet 915
  while (DAC->STATUS.bit.SYNCBUSY)
    ;

  DAC->CTRLA.bit.ENABLE = 1;
  while (DAC->STATUS.bit.SYNCBUSY)
    ;  //attend que le bit de synchronisation avec le CPU Atmel | SMART SAM D21 [DATASHEET] 89
}

void setupADC(void) {  //source https://blog.thea.codes/reading-analog-values-with-the-samd-adc/
  /* Enable the APB clock for the ADC. */
  PM->APBCMASK.reg |= PM_APBCMASK_ADC;
  /* Enable GCLK1 for the ADC */
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK1 | GCLK_CLKCTRL_ID_ADC;

  /* Wait for bus synchronization. */
  while (GCLK->STATUS.bit.SYNCBUSY) {};
  uint32_t bias = (*((uint32_t *)ADC_FUSES_BIASCAL_ADDR) & ADC_FUSES_BIASCAL_Msk) >> ADC_FUSES_BIASCAL_Pos;
  uint32_t linearity = (*((uint32_t *)ADC_FUSES_LINEARITY_0_ADDR) & ADC_FUSES_LINEARITY_0_Msk) >> ADC_FUSES_LINEARITY_0_Pos;
  linearity |= ((*((uint32_t *)ADC_FUSES_LINEARITY_1_ADDR) & ADC_FUSES_LINEARITY_1_Msk) >> ADC_FUSES_LINEARITY_1_Pos) << 5;

  /* Wait for bus synchronization. */
  while (ADC->STATUS.bit.SYNCBUSY) {};

  /* Write the calibration data. */
  ADC->CALIB.reg = ADC_CALIB_BIAS_CAL(bias) | ADC_CALIB_LINEARITY_CAL(linearity);
  /* Wait for bus synchronization. */
  while (ADC->STATUS.bit.SYNCBUSY) {};

  /* Use the internal VCC reference. This is 1/2 of what's on VCCA.
   since VCCA is typically 3.3v, this is 1.65v.
*/
  ADC->REFCTRL.reg = ADC_REFCTRL_REFSEL_INTVCC1;

  /* Only capture one sample. The ADC can actually capture and average multiple
   samples for better accuracy, but there's no need to do that for this
   example.
*/
  ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1;

  /* Set the clock prescaler to 512, which will run the ADC at
   8 Mhz / 512 = 31.25 kHz.
   Set the resolution to 12bit.
*/
  ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV4 | ADC_CTRLB_RESSEL_8BIT;

  /* Configure the input parameters.

   - GAIN_DIV2 means that the input voltage is halved. This is important
     because the voltage reference is 1/2 of VCCA. So if you want to
     measure 0-3.3v, you need to halve the input as well.

   - MUXNEG_GND means that the ADC should compare the input value to GND.

   - MUXPOST_PIN3 means that the ADC should read from AIN3, or PB09.
     This is A2 on the Feather M0 board.
*/
  ADC->INPUTCTRL.reg = ADC_INPUTCTRL_GAIN_DIV2 | ADC_INPUTCTRL_MUXNEG_GND | ADC_INPUTCTRL_MUXPOS_PIN3;
  /* Set PB09 as an input pin. */
  PORT->Group[1].DIRCLR.reg = PORT_PB08;

  /* Enable the peripheral multiplexer for PB09. */
  PORT->Group[1].PINCFG[8].reg |= PORT_PINCFG_PMUXEN;

  /* Set PB09 to function B which is analog input. */
  PORT->Group[1].PMUX[4].reg = PORT_PMUX_PMUXO_B;
  /* Wait for bus synchronization. */
  while (ADC->STATUS.bit.SYNCBUSY) {};

  /* Enable the ADC. */
  ADC->CTRLA.bit.ENABLE = true;
}

uint8_t ADC_mesurement(void)
{
    /* Wait for bus synchronization. */
while (ADC->STATUS.bit.SYNCBUSY) {};

/* Start the ADC using a software trigger. */
ADC->SWTRIG.bit.START = true;

/* Wait for the result ready flag to be set. */
while (ADC->INTFLAG.bit.RESRDY == 0);

/* Clear the flag. */
ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY;

/* Read the value. */
return ADC->RESULT.reg;
}

void setupTimer_DAC(void) {
  PM->APBCMASK.reg |= PM_APBCMASK_TC3;  //active la clock TC3 : PM est un pointeur vers la struct Pm définie dans pm.h ligne 521 comme suit :" __IO PM_APBCMASK_Type          APBCMASK;" APBCMASK est donc un membre de la struct Pm et on active la Clock grâce à un masque binaire qui active le douziéme bit (Atmel | SMART SAM D21 [DATASHEET] 127) grâce à une porte OU appliquée au registre APBC

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(TC3_GCLK_ID) | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN;
  //configure TC3 : on applique succesivement plusieurs masque binaire pour tout les appliquer au registre CLKCTR qui est un membre d'une struct qu'on accede via le pointer GCLK.
  //CLKCTRL est un membre de la struct Gclk (gclk.h ligne 302). C'est un registre de 16 bits (Atmel | SMART SAM D21 [DATASHEET] 106)
  //GCLK_CLKCTRL_ID est un macro définit dans gclk.h ligne 112 qui permet de traîter le paramètre d'entrée pour que seul les 6bits de poid faible soit lu en effet dans gclk.h on applique une opération logique ET avec masque binaire de 0x3Ful (ul signifie unsigned long) soit 0011 1111 pour que seul les 6 bits de poid faibles restent pour ne pas affecter les masques suivants à noter que l'ID de GCLK TC3 vaut 0x1B (Atmel | SMART SAM D21 [DATASHEET] 108) soit 0001 1011
  //donc dans ce cas-ci le traitement n'était pas nécessaire
  //GCLK_CLKCTRL_GEN_GCLK0 est un masque binaire définit ligne 198 de gclk.h 0x0ul (unsigned long) est l'adresse du premier générateur de clock "generic" sur les 8
  //GCLK_CLKCTRL_CLKEN un macro qui permet d'activer la clock (enable) si il est à 1 c'est le 15éme bit du registre CLKCTRL il est définit ligne 207 de gclk.h il vaut 1 dans la librairie mais pour affecter que le bon bit qui est le 15éme, on lui applique un bit shift grâce à l'opération << de 14 (définit par GCLK_CLKCTRL_CLKEN_Pos ligne 206 de gclk.h) ce qui fait que GCLK_CLKCTRL_CLKEN vaut en réalite 100 0000 0000 0000 c'est donc bien le 15ème bit qui est mis à 1.
  //Cette méthode permet que le code soit compréhensible pour l'homme
  //le .reg est une union définit ligne 228 gclk.h. une union est une struct qui peut contenir qu'un seul de ses membres à la fois, dans ce cas il y a qu'un membre qui est une struct donc c'est de l'optimisation de mémoire.

  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  //on attends que la clock soit synchronisée

  TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;  //software reset

  while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
    ;  //on attends que la clock soit synchronisée

  TC3->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_MFRQ | TC_CTRLA_PRESCALER_DIV1;
  //on configure TC3 en compteur de 16 bits
  //on accède à la struct COUNT16, au registre CTRLA et on applique un porte logique OU avec plusieur masque binaire pour mettre la clock en compteur de 16 bits, et choisir un prescaler de 0
  TC3->COUNT16.CC[0].reg = 48;

  while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
    ;

  TC3->COUNT16.INTENSET.bit.MC0 = 1;

  NVIC_EnableIRQ(TC3_IRQn);

  TC3->COUNT16.CTRLA.bit.ENABLE = 1;

  while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
    ;
}

void generateSineWave(void) {
  for (int i = 0; i < sample_size; i++) {
    float angle = (2.0 * M_PI * i) / sample_size;
    sineTable[i] = (uint16_t)(AMPLITUDE * sin(angle) + OFFSET);
  }
}



// Routine d'interruption rapide et optimisée
void TC3_Handler(void) {
  static volatile uint16_t currentIndex = 0;
  TC3->COUNT16.INTFLAG.bit.MC0 = 1;  // Effacer immédiatement le flag
  delayMicroseconds(2);              //réduire la fréquence pour qu'elle soit dans la bande passante
  DAC->DATA.reg = sineTable[currentIndex++];
  if (currentIndex >= sample_size) {
    currentIndex = 0;
  }
}
