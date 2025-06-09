#include "samd21/include/samd21g18a.h"
#include <stdint.h>
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

void setupDAC_8bit() {
  // Activer l'horloge du DAC
  PM->APBCMASK.reg |= PM_APBCMASK_DAC;

  // Sélectionner le générateur d'horloge pour le DAC
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(DAC_GCLK_ID) |
                      GCLK_CLKCTRL_GEN_GCLK0 |
                      GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.bit.SYNCBUSY);

  // Réinitialiser le DAC
  DAC->CTRLA.bit.SWRST = 1;
  while (DAC->CTRLA.bit.SWRST || DAC->STATUS.bit.SYNCBUSY);

  // Configurer la référence de tension et activer la sortie
  DAC->CTRLB.reg = DAC_CTRLB_EOEN | DAC_CTRLB_REFSEL_AVCC;

  // Activer le DAC
  DAC->CTRLA.bit.ENABLE = 1;
  while (DAC->STATUS.bit.SYNCBUSY);
}

inline void analogWriteDAC_8bit(uint8_t value) {
  DAC->DATA.reg = value << 2;  // Écriture 8 bits → converti en 10 bits (alignement à droite)
  while (DAC->STATUS.bit.SYNCBUSY);
}
