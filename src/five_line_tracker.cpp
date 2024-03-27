#include "five_line_tracker.h"

#include <Arduino.h>
#include <Wire.h>
#include <stddef.h>

namespace {
enum MemoryAddress : uint8_t {
  kMemoryAddressDeviceId = 0x00,
  kMemoryAddressVersion = 0x01,
  kMemoryAddressAnalogValues = 0x10,
  kMemoryAddressDigitalValues = 0x1A,
  kMemoryAddressHighThresholds = 0x1C,
  kMemoryAddressLowThresholds = 0x26,
};

}

FiveLineTracker::FiveLineTracker(const uint8_t i2c_address) : i2c_address_(i2c_address) {}

bool FiveLineTracker::Initialize(const TwoWire* wire) {
  wire_ = wire;
  return true;
}

uint8_t FiveLineTracker::DeviceId() {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kMemoryAddressDeviceId);
  wire_->endTransmission();

  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(1));
  if (wire_->available()) {
    return wire_->read();
  }

  return 0x00;
}

uint8_t FiveLineTracker::Version() {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kMemoryAddressVersion);
  wire_->endTransmission();

  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(1));
  if (wire_->available()) {
    return wire_->read();
  }

  return 0x00;
}

void FiveLineTracker::HighThresholds(const uint16_t high_thresholds[kLineNumber]) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kMemoryAddressHighThresholds);
  wire_->write(reinterpret_cast<const char*>(high_thresholds), kLineNumber << 1);
  wire_->endTransmission();
}

void FiveLineTracker::HighThreshold(const uint8_t channel, const uint16_t high_threshold) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kMemoryAddressHighThresholds + (channel << 1));
  wire_->write(reinterpret_cast<const char*>(high_threshold), sizeof(high_threshold));
  wire_->endTransmission();
}

void FiveLineTracker::LowThresholds(const uint16_t low_thresholds[kLineNumber]) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kMemoryAddressLowThresholds);
  wire_->write(reinterpret_cast<const char*>(low_thresholds), kLineNumber << 1);
  wire_->endTransmission();
}

void FiveLineTracker::LowThreshold(const uint8_t channel, const uint16_t low_threshold) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kMemoryAddressLowThresholds + (channel << 1));
  wire_->write(reinterpret_cast<const char*>(low_threshold), sizeof(low_threshold));
  wire_->endTransmission();
}

void FiveLineTracker::AnalogValues(uint16_t analog_values[kLineNumber]) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kMemoryAddressAnalogValues);
  wire_->endTransmission();

  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(kLineNumber << 1));
  wire_->readBytes(reinterpret_cast<uint8_t*>(analog_values), kLineNumber << 1);
}

uint16_t FiveLineTracker::AnalogValue(uint8_t channel) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kMemoryAddressAnalogValues + (channel << 1));
  wire_->endTransmission();

  wire_->requestFrom(i2c_address_, sizeof(uint16_t));
  uint16_t value = 0;
  wire_->readBytes(reinterpret_cast<uint8_t*>(value), sizeof(uint16_t));
  return value;
}

uint8_t FiveLineTracker::DigitalValues() {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kMemoryAddressDigitalValues);
  wire_->endTransmission();

  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(1));
  if (wire_->available()) {
    return wire_->read();
  }

  return 0x00;
}

uint8_t FiveLineTracker::DigitalValue(uint8_t channel) { return (DigitalValues() >> channel) & 0x01; }