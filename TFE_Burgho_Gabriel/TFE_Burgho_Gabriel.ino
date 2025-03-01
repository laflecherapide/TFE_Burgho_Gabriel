
//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

void setup() {
    Serial.begin(115200);
    generateSineWave();
    setupDAC();
    setupTimer();
}
 
void loop() {
    // Rien à faire ici, tout est géré par interruptions
}