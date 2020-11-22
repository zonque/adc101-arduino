#pragma once

#include <Arduino.h>
#include <Wire.h>

enum ADC101CMode
{
  ADC101C_MODE_AUTO_OFF = (0 << 5),
  ADC101C_MODE_AUTO_32 = (1 << 5),
  ADC101C_MODE_AUTO_64 = (2 << 5),
  ADC101C_MODE_AUTO_128 = (3 << 5),
  ADC101C_MODE_AUTO_256 = (4 << 5),
  ADC101C_MODE_AUTO_512 = (5 << 5),
  ADC101C_MODE_AUTO_1024 = (6 << 5),
  ADC101C_MODE_AUTO_2048 = (7 << 5),

  ADC101C_MODE_ALERT_HOLD = (1 << 4),
  ADC101C_MODE_ALERT_FLAG_ENABLED = (1 << 3),
  ADC101C_MODE_ALERT_PIN_ENABLED = (1 << 2),
  ADC101C_MODE_ALERT_PIN_ACTIVE_HIGH = (1 << 0),
};

class ADC101C
{
public:
  bool begin(TwoWire *pWire, uint8_t addr);
  bool init();
  uint16_t read();
  void setMode(int mode);

  // For auto mode
  void setAlertLimits(uint16_t under, uint16_t over);
  void setAlertHysteresis(uint16_t val);

  bool underAlert(bool clear = true);
  bool overAlert(bool clear = true);

  uint16_t highest(bool clear = true);
  uint16_t lowest(bool clear = true);

private:
  bool alert(uint8_t mask, bool clear);
  uint16_t readRegister(uint8_t reg, uint8_t bytes);
  void writeRegister(uint8_t reg, uint16_t val, uint8_t bytes);
  uint16_t readConversionRegister(uint8_t reg, bool clear);

  uint8_t i2cAddr;
  uint8_t currentRegister;
  TwoWire *wire;
};
