/*
config clock {
  To connect a Generic Clock Generator to a peripheral within the device, a Generic Clock Channel is used. Each
peripheral or peripheral group has an associated Generic Clock Channel, which serves as the clock input for
the peripheral(s). To supply a clock to the peripheral module(s), the associated channel must be connected to a
running Generic Clock Generator and the channel enabled.
A Basic Timer is a simple application where compare match operations is used to determine when a specific period
has elapsed. In Basic Timer operations, one or more values in the module's Compare/Capture registers are used
to specify the time (as a number of prescaled GCLK cycles) when an action should be taken by the microcontroller.
This can be an Interrupt Service Routine (ISR), event generator via the event system, or a software flag that is
polled via the user application.

}
config dmac {
  12 channels
  DMA transfer is based on descriptors, which holds
transfer properties such as the source and destination addresses, transfer counter and other additional transfer
control information. The descriptors can be static or linked. When static, a single block transfer is performed.
When linked, a number of transfer descriptors can be used to enable multiple block transfers within a single DMA
transaction.
The DMAC in each device consists of several DMA channels, which along with the transfer descriptors defines the
data transfer properties.
● The transfer control descriptor defines the source and destination addresses, source and destination address
increment settings, the block transfer count and event output condition selection.
● Dedicated channel registers control the peripheral trigger source, trigger mode settings, event input actions
and channel priority level settings.
With a successful DMA resource allocation, a dedicated DMA channel will be assigned. The channel will be
occupied until the DMA resource is freed. A DMA resource handle is used to identify the specific DMA resource.
When there are multiple channels with active requests, the arbiter prioritizes the channels requesting access to the
bus.
Both an interrupt callback and an peripheral event can be triggered by the DMA transfer. Three types of callbacks
are supported by the DMA driver: transfer complete, channel suspend and transfer error. Each of these callback
types can be registered and enabled for each channel independently through the DMA driver API.
The DMAC module can also generate events on transfer complete. Event generation is enabled through the DMA
channel, event channel configuration and event user multiplexing is done through the events driver.
The DMAC can generate events in the below cases:
● When a block transfer is complete
● When each beat transfer within a block transfer is complete

}
config clock dma
config DAC
*/

//****************LIBRAIRIE*****************
#include "talkie_walkie.h"
#include <Arduino.h>
#include <math.h>
 
#define TABLE_SIZE 128
#define AMPLITUDE 512
#define OFFSET 512
 
uint16_t sineTable[TABLE_SIZE];//tableau de int non signé (>1) de 16 bits

 
void generateSineWave() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        float angle = (2.0 * M_PI * i) / TABLE_SIZE;
        sineTable[i] = (uint16_t)(AMPLITUDE * sin(angle) + OFFSET);
    }
}
 
void setupDAC() {
    DAC->CTRLA.bit.ENABLE = 0;//DAC est un pointer et on accede au membre CTRLA.bit.ENABLE grace à l'opérateur "->" et on le met à 0 ce qui désactive le DAC ce qui est nécessaire pour le configurer. CTRLA est une struct bit est une sous struct et ENABLE est un membre de la sous struct bit.
    while (DAC->STATUS.bit.SYNCBUSY);
 
    DAC->CTRLB.reg = DAC_CTRLB_EOEN | DAC_CTRLB_REFSEL(1); //on choisit la reference du comparateur source Atmel | SMART SAM D21 Datasheet p915
    while (DAC->STATUS.bit.SYNCBUSY);
 
    DAC->CTRLA.bit.ENABLE = 1;
    while (DAC->STATUS.bit.SYNCBUSY);//attend que le bit de synchronisation avec le CPU Atmel | SMART SAM D21 [DATASHEET] 89
}

void setupTimer() {
    PM->APBCMASK.reg |= PM_APBCMASK_TC3;//active la clock TC3 : PM est un pointeur vers la struct Pm définie dans pm.h ligne 521 comme suit :" __IO PM_APBCMASK_Type          APBCMASK;" APBCMASK est donc un membre de la struct Pm et on active la Clock grâce à un masque binaire qui active le douziéme bit (Atmel | SMART SAM D21 [DATASHEET] 127) grâce à une porte OU appliquée au registre APBC

      GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(TC3_GCLK_ID) |
                          GCLK_CLKCTRL_GEN_GCLK0 |
                          GCLK_CLKCTRL_CLKEN;
//configure TC3 : on applique succesivement plusieurs masque binaire pour tout les appliquer au registre CLKCTR qui est un membre d'une struct qu'on accede via le pointer GCLK. 
//CLKCTRL est un membre de la struct Gclk (gclk.h ligne 302) 
//GCLK_CLKCTRL_ID est un macro définit dans gclk.h ligne 112 qui permet de traîter le paramètre d'entrée pour que seul les 6bits de poid faible soit lu en effet dans gclk.h on applique une opération logique ET avec masque binaire de 0x3Ful (ul signifie unsigned long) soit 0011 1111 pour que seul les 6 bits de poid faibles restent pour ne pas affecter les masques suivants à noter que l'ID de GCLK TC3 vaut 0x1B (Atmel | SMART SAM D21 [DATASHEET] 108) soit 0001 1011
//donc dans ce cas-ci le traitement n'était pas nécessaire
//GCLK_CLKCTRL_GEN_GCLK0 est un masque binaire définit ligne 198 de gclk.h 0x0ul (unsigned long) est l'adresse du premier générateur de clock
//
    while (GCLK->STATUS.bit.SYNCBUSY);
 
    TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
 
    TC3->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 |
                             TC_CTRLA_WAVEGEN_MFRQ |
                             TC_CTRLA_PRESCALER_DIV1;
 
    // Pour environ 44 kHz d'échantillonnage : (48MHz / 44kHz ≈ 1090)
    TC3->COUNT16.CC[0].reg = 48;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
 
    TC3->COUNT16.INTENSET.bit.MC0 = 1;
    NVIC_EnableIRQ(TC3_IRQn);
 
    TC3->COUNT16.CTRLA.bit.ENABLE = 1;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
}
// Routine d'interruption rapide et optimisée
void TC3_Handler() {
  static volatile uint16_t currentIndex = 0;
    TC3->COUNT16.INTFLAG.bit.MC0 = 1; // Effacer immédiatement le flag
 
    DAC->DATA.reg = sineTable[currentIndex++];
    if (currentIndex >= TABLE_SIZE) 
    {
      currentIndex = 0;
    }
}
 
void setup() {
    Serial.begin(115200);
    generateSineWave();
    setupDAC();
    setupTimer();
}
 
void loop() {
    // Rien à faire ici, tout est géré par interruptions
}