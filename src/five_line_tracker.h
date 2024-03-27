#pragma once

#include <Wire.h>
#include <inttypes.h>

class FiveLineTracker {
 public:
  static constexpr uint8_t kDefaultI2cAddress = 0x50;
  static constexpr uint8_t kLineNumber = 5;

  struct AnalogValues {
    uint16_t analog_values[kLineNumber] = {0};
  };

  FiveLineTracker(const uint8_t i2c_address = kDefaultI2cAddress);
  bool Initialize(const TwoWire* wire);
  uint8_t DeviceId();
  uint8_t Version();
  void HighThresholds(const uint16_t high_thresholds[kLineNumber]);
  void HighThreshold(const uint8_t channel, const uint16_t high_threshold);
  void LowThresholds(const uint16_t low_thresholds[kLineNumber]);
  void LowThreshold(const uint8_t channel, const uint16_t low_threshold);
  void AnalogValues(uint16_t analog_values[kLineNumber]);
  uint16_t AnalogValue(uint8_t channel);
  uint8_t DigitalValues();
  uint8_t DigitalValue(uint8_t channel);

 private:
  const uint8_t i2c_address_ = kLineNumber;
  const TwoWire* wire_ = nullptr;
};
