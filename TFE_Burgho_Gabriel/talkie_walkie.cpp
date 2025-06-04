//#include "samd21/include/samd21g18a.h"
//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

uint8_t buffer_parler[250];
uint8_t buffer_entendre[250];

static inline void wait_cycles(uint32_t n) {//chatgpt sauf commentaire : delay bas niveau 
//static signifie que c'est stocké en dehors de la pile, donc 
//inline signifie que le compilateur mette le bout de code à chaque fois qu'on appel la fonction au lieu d'appelé la fonction
    while(n--) {
        __asm__ volatile ("nop"); 
        //__asm__ signifie que c'est de l'assembleur
        //volatile signifie que elle est accessible
        //"nop" signifie pour le processeur de ne rien faire
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
  ADC->INPUTCTRL.reg = ADC_INPUTCTRL_GAIN_DIV2 | ADC_INPUTCTRL_MUXNEG_GND | ADC_INPUTCTRL_MUXPOS_PIN2;
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

void TC3_Handler(void) 
{//entree ADC remplir buffer
  static volatile uint16_t currentIndex = 0;
  buffer_parler[currentIndex] = analogRead(A1);
  currentIndex++;
  TC3->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
  if (currentIndex >= sample_size) 
  {//revenir à la valeur 0 du tableau
    currentIndex = 0;
  }
}

void TC4_Handler(void) 
{//sortie DAC vider buffer
  static volatile uint16_t currentIndex = 0;
  analogWriteDAC(buffer_entendre[currentIndex]);
  currentIndex++;
  TC4->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
  if (currentIndex >= sample_size) 
  {//vider un buffer
    currentIndex = 0;
  }
}

void generatesample(void)
{
  for (int i = 0; i < 250; i++)
  {
    buffer_entendre[i] = i;
  }
}

void setupDAC() {//chatgpt
  // Enable the DAC peripheral clock
  PM->APBCMASK.reg |= PM_APBCMASK_DAC;

  // Configure the GCLK for DAC
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(DAC_GCLK_ID) |
                      GCLK_CLKCTRL_GEN_GCLK0 | // Use Generic Clock Generator 0
                      GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.bit.SYNCBUSY);

  // Reset DAC
  DAC->CTRLA.bit.SWRST = 1;
  while (DAC->CTRLA.bit.SWRST);

  // Set voltage reference to AVCC
  DAC->CTRLB.reg = DAC_CTRLB_EOEN | DAC_CTRLB_REFSEL_AVCC;

  // Enable DAC
  DAC->CTRLA.bit.ENABLE = 1;
  while (DAC->STATUS.bit.SYNCBUSY);
}

inline void analogWriteDAC(uint16_t value) {//chatgpt
  if (value > 4095) value = 4095; // Clamp to 12-bit max

  DAC->DATA.reg = value;
  while (DAC->STATUS.bit.SYNCBUSY);
}