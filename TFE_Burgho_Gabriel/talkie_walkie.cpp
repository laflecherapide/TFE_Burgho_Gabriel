#include "delay.h"

//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

uint16_t sineTable[TABLE_SIZE];//tableau de int non signé (>1) de 16 bits 

void setupTimer(void) {
    PM->APBCMASK.reg |= PM_APBCMASK_TC3;//active la clock TC3 : PM est un pointeur vers la struct Pm définie dans pm.h ligne 521 comme suit :" __IO PM_APBCMASK_Type          APBCMASK;" APBCMASK est donc un membre de la struct Pm et on active la Clock grâce à un masque binaire qui active le douziéme bit (Atmel | SMART SAM D21 [DATASHEET] 127) grâce à une porte OU appliquée au registre APBC

      GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(TC3_GCLK_ID) |
                          GCLK_CLKCTRL_GEN_GCLK0 |
                          GCLK_CLKCTRL_CLKEN;
//configure TC3 : on applique succesivement plusieurs masque binaire pour tout les appliquer au registre CLKCTR qui est un membre d'une struct qu'on accede via le pointer GCLK. 
//CLKCTRL est un membre de la struct Gclk (gclk.h ligne 302). C'est un registre de 16 bits (Atmel | SMART SAM D21 [DATASHEET] 106) 
//GCLK_CLKCTRL_ID est un macro définit dans gclk.h ligne 112 qui permet de traîter le paramètre d'entrée pour que seul les 6bits de poid faible soit lu en effet dans gclk.h on applique une opération logique ET avec masque binaire de 0x3Ful (ul signifie unsigned long) soit 0011 1111 pour que seul les 6 bits de poid faibles restent pour ne pas affecter les masques suivants à noter que l'ID de GCLK TC3 vaut 0x1B (Atmel | SMART SAM D21 [DATASHEET] 108) soit 0001 1011
//donc dans ce cas-ci le traitement n'était pas nécessaire
//GCLK_CLKCTRL_GEN_GCLK0 est un masque binaire définit ligne 198 de gclk.h 0x0ul (unsigned long) est l'adresse du premier générateur de clock "generic" sur les 8
//GCLK_CLKCTRL_CLKEN un macro qui permet d'activer la clock (enable) si il est à 1 c'est le 15éme bit du registre CLKCTRL il est définit ligne 207 de gclk.h il vaut 1 dans la librairie mais pour affecter que le bon bit qui est le 15éme, on lui applique un bit shift grâce à l'opération << de 14 (définit par GCLK_CLKCTRL_CLKEN_Pos ligne 206 de gclk.h) ce qui fait que GCLK_CLKCTRL_CLKEN vaut en réalite 100 0000 0000 0000 c'est donc bien le 15ème bit qui est mis à 1.
//Cette méthode permet que le code soit compréhensible pour l'homme
//le .reg est une union définit ligne 228 gclk.h. une union est une struct qui peut contenir qu'un seul de ses membres à la fois, dans ce cas il y a qu'un membre qui est une struct donc c'est de l'optimisation de mémoire.

    while (GCLK->STATUS.bit.SYNCBUSY);//on attends que la clock soit synchronisée
 
    TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;//software reset

    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);//on attends que la clock soit synchronisée
 
    TC3->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 |
                             TC_CTRLA_WAVEGEN_MFRQ |
                             TC_CTRLA_PRESCALER_DIV1;
//on configure TC3 en compteur de 16 bits
//on accède à la struct COUNT16, au registre CTRLA et on applique un porte logique OU avec plusieur masque binaire pour mettre la clock en compteur de 16 bits, et choisir un prescaler de 0
    TC3->COUNT16.CC[0].reg = 48;

    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
 
    TC3->COUNT16.INTENSET.bit.MC0 = 1;
    
    NVIC_EnableIRQ(TC3_IRQn);
 
    TC3->COUNT16.CTRLA.bit.ENABLE = 1;
    
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
}

void generateSineWave(void) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        float angle = (2.0 * M_PI * i) / TABLE_SIZE;
        sineTable[i] = (uint16_t)(AMPLITUDE * sin(angle) + OFFSET);
    }
}

void setupDAC(void) {
    DAC->CTRLA.bit.ENABLE = 0;//DAC est un pointer et on accede au membre CTRLA.bit.ENABLE grace à l'opérateur "->" et on le met à 0 ce qui désactive le DAC ce qui est nécessaire pour le configurer. CTRLA est une struct bit est une sous struct et ENABLE est un membre de la sous struct bit.
    while (DAC->STATUS.bit.SYNCBUSY);
 
    DAC->CTRLB.reg = DAC_CTRLB_EOEN | DAC_CTRLB_REFSEL(1); //on choisit la reference du comparateur source Atmel | SMART SAM D21 Datasheet 915
    while (DAC->STATUS.bit.SYNCBUSY);
 
    DAC->CTRLA.bit.ENABLE = 1;
    while (DAC->STATUS.bit.SYNCBUSY);//attend que le bit de synchronisation avec le CPU Atmel | SMART SAM D21 [DATASHEET] 89
}

// Routine d'interruption rapide et optimisée
void TC3_Handler(void) {
  static volatile uint16_t currentIndex = 0;
    TC3->COUNT16.INTFLAG.bit.MC0 = 1; // Effacer immédiatement le flag
    delayMicroseconds(2);//réduire la fréquence pour qu'elle soit dans la bande passante
    DAC->DATA.reg = sineTable[currentIndex++];
    if (currentIndex >= TABLE_SIZE) 
    {
      currentIndex = 0;
    }
}

