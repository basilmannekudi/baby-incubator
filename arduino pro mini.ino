#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     500
PulseOximeter pox;
int sens = 0;
uint32_t tsLastReport = 0;
void setup() {
  Serial.begin(9600); //Initialize Serial Communication
  if (!pox.begin()) {
        sens = 1;
        for(;;);
    } else {
        sens = 0;
    }
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

void loop() {
  pox.update();
 if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
  Serial.print(pox.getHeartRate());
  Serial.print(",");
  Serial.print(pox.getSpO2());
  Serial.print(",");
  Serial.println(sens);
        tsLastReport = millis();
    }
}
