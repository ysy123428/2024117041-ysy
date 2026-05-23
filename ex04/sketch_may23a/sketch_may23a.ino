#define TOUCH_PIN 4
#define LED_PIN 2

bool ledState = LOW;
bool lastTouch = false;
unsigned long debounce = 0;
const int debounceMs = 150;

// 触碰后稳定峰值333
#define TOUCH_PEAK 320

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  int touchValue = touchRead(TOUCH_PIN);
  Serial.println(touchValue);

  // 核心反向判定：数值变高、≥320 才算触摸触发
  bool currentTouch = (touchValue >= TOUCH_PEAK);

  // 只检测刚触碰的一瞬间，防抖防误触
  if (millis() - debounce > debounceMs) {
    if (currentTouch == true && lastTouch == false) {
      // 翻转D2 LED
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      debounce = millis();
      Serial.println("🎉 触摸触发！D2亮灭切换");
    }
  }

  lastTouch = currentTouch;
  delay(20);
}