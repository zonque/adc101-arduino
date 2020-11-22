#include "ADC101C.h"

enum
{
  ADC101C_REG_CONVERSION_RESULT = 0x0,
  ADC101C_REG_ALERT_STATUS = 0x1,
  ADC101C_REG_CONFIGURATION = 0x2,
  ADC101C_REG_LOW_LIMIT = 0x3,
  ADC101C_REG_HIGH_LIMIT = 0x4,
  ADC101C_REG_HYSTERESIS = 0x5,
  ADC101C_REG_LOWEST_CONVERSION = 0x6,
  ADC101C_REG_HIGHEST_CONVERSION = 0x7,
};

bool ADC101C::begin(TwoWire *pWire, uint8_t addr)
{
  wire = pWire;
  i2cAddr = addr;

  wire->begin();
  wire->beginTransmission(i2cAddr);
  return wire->endTransmission() == 0;
}

uint16_t ADC101C::read()
{
  return readConversionRegister(ADC101C_REG_CONVERSION_RESULT, false);
}

void ADC101C::setMode(int mode)
{
  writeRegister(ADC101C_REG_CONFIGURATION, mode, 1);
}

void ADC101C::setAlertLimits(uint16_t under, uint16_t over)
{
  writeRegister(ADC101C_REG_LOW_LIMIT, under, 2);
  writeRegister(ADC101C_REG_HIGH_LIMIT, over, 2);
}

void ADC101C::setAlertHysteresis(uint16_t val)
{
  writeRegister(ADC101C_REG_HYSTERESIS, (val & 0x3ff) << 2, 2);
}

bool ADC101C::underAlert(bool clear)
{
  return alert(0x01, clear);
}

bool ADC101C::overAlert(bool clear)
{
  return alert(0x02, clear);
}

uint16_t ADC101C::highest(bool clear)
{
  return readConversionRegister(ADC101C_REG_HIGHEST_CONVERSION, clear);
}

uint16_t ADC101C::lowest(bool clear)
{
  return readConversionRegister(ADC101C_REG_LOWEST_CONVERSION, clear);
}

// private

bool ADC101C::alert(uint8_t mask, bool clear)
{
  uint8_t val = readRegister(ADC101C_REG_ALERT_STATUS, 1);

  if ((val & mask) == mask) {
    if (clear)
      writeRegister(ADC101C_REG_ALERT_STATUS, mask, 1);

    return true;
  }

  return false;
}

uint16_t ADC101C::readRegister(uint8_t reg, uint8_t bytes)
{
  if (reg != currentRegister) {
    wire->beginTransmission(i2cAddr);
    wire->write(reg);
    wire->endTransmission();
    currentRegister = reg;
  }

  uint16_t val = 0;
  wire->beginTransmission(i2cAddr);
  wire->requestFrom(i2cAddr, bytes);

  while (bytes--)
  {
    val <<= 8;
    val |= wire->read() & 0xff;
  }

  wire->endTransmission();

  return val;
}

void ADC101C::writeRegister(uint8_t reg, uint16_t val, uint8_t bytes)
{
  wire->beginTransmission(i2cAddr);

  wire->write(reg);
  currentRegister = reg;

  for (int i = bytes - 1; i >= 0; i--)
    wire->write((val >> (i * 8)) & 0xff);

  wire->endTransmission();
}

uint16_t ADC101C::readConversionRegister(uint8_t reg, bool clear)
{
  uint16_t val = (readRegister(reg, 2) >> 2) & 0x3ff;

  if (clear)
    writeRegister(reg, 0xfff, 2);

  return val;
}
