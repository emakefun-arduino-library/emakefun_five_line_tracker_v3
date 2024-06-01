#include <five_line_tracker.h>

#define INFINITE_LOOP_ON_FAILURE InfiniteLoopOnFailure(__FUNCTION__, __LINE__)

namespace {

emakefun::FiveLineTracker g_five_line_tracker;

void InfiniteLoopOnFailure(const char* function, const uint32_t line_number) {
  Serial.println(String(F("entering an infinite loop due to failure in ")) + function + ", at line number: " + line_number);
  while (true) {
    yield();
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);
  Serial.println(F("setup"));

  Wire.begin();

  const auto ret = g_five_line_tracker.Initialize();

  if (emakefun::FiveLineTracker::kOK == ret) {
    Serial.println(F("five line tracker initialization successful"));
  } else {
    Serial.print(F("five line tracker initialization failed: "));
    Serial.println(ret);
    INFINITE_LOOP_ON_FAILURE;
  }

  Serial.println(String(F("five line tracker firmware version: ")) +
                 static_cast<uint32_t>(g_five_line_tracker.FirmwareVersion()));

  const uint16_t high_thresholds[emakefun::FiveLineTracker::kLineNumber] = {400, 400, 400, 400, 400};
  g_five_line_tracker.HighThresholds(high_thresholds);

  const uint16_t low_thresholds[emakefun::FiveLineTracker::kLineNumber] = {200, 200, 200, 200, 200};
  g_five_line_tracker.LowThresholds(low_thresholds);

  Serial.println(F("setup successful"));
}

void loop() {
  uint16_t analog_values[emakefun::FiveLineTracker::kLineNumber] = {0};
  g_five_line_tracker.AnalogValues(analog_values);

  const uint8_t digital_values = g_five_line_tracker.DigitalValues();

  String log(F("analog values: "));
  for (uint8_t i = 0; i < emakefun::FiveLineTracker::kLineNumber; i++) {
    log += i;
    log += F(":");
    log += analog_values[i];  // 获取一路传感器的值加到日志log变量中
    log += F(", ");
  }

  log += F("digital values: 0x");
  log += String(digital_values, HEX);
  Serial.println(log);
}
