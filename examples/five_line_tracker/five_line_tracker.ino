#include <Arduino.h>
#include <five_line_tracker.h>

FiveLineTracker g_five_line_tracker(0x50);

void setup() {
  Serial.begin(115200);
  Serial.println("setup");

  Wire.begin();
  g_five_line_tracker.Initialize(&Wire);

  const uint16_t high_thresholds[FiveLineTracker::kLineNumber] = {450, 400, 410, 400, 430};
  g_five_line_tracker.HighThresholds(high_thresholds);

  const uint16_t low_thresholds[FiveLineTracker::kLineNumber] = {320, 330, 350, 310, 300};
  g_five_line_tracker.LowThresholds(low_thresholds);
}

void loop() {
  uint16_t analog_values[FiveLineTracker::kLineNumber] = {0};
  g_five_line_tracker.AnalogValues(analog_values);

  const uint8_t digital_values = g_five_line_tracker.DigitalValues();

  String log("analog values: ");
  for (uint8_t i = 0; i < FiveLineTracker::kLineNumber; i++) {
    log += i;
    log += ":";
    log += analog_values[i];  // 获取一路传感器的值加到日志log变量中
    log += ", ";
  }

  log += "digital values: 0x";
  log += String(digital_values, HEX);
  Serial.println(log);
}
