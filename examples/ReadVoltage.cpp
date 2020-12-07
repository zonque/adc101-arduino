#include <Arduino.h>
#include <Wire.h>
#include "ADC101C.h"

ADC101C adc;

void setup(void)
{
  Wire.begin(D2, D1);
  Serial.begin(115200);

  while (!adc.begin(&Wire, 0x50)) {
    Serial.println("Cannot init ADC101C");
    delay(1000);
  }

  adc.setMode(ADC101C_MODE_AUTO_512 | ADC101C_MODE_ALERT_FLAG_ENABLED | ADC101C_MODE_ALERT_PIN_ENABLED | ADC101C_MODE_ALERT_HOLD);
  adc.setAlertLimits(0x100, 0x300);
}

void loop(void) {
  Serial.printf("Last conversion: 0x%03x, highest 0x%03x, lowest %03x, under alert %s, over alert %s\n",
    adc.read(), adc.highest(true), adc.lowest(true),
    adc.underAlert(true) ? "yes" : "no",
    adc.overAlert(true)  ? "yes" : "no");

  delay(1000);
}
