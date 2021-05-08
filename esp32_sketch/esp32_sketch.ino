/**
 * The sketch cotains two main parts:
 * - Serial console for controlling LED
 * - BLE GATT service
 */

static const uint8_t blue_led = 2; //LED pin on ESP32 DEVKIT V1 DOIT board

void console_routine(void) {
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == '0')
      digitalWrite(blue_led, LOW); //turn LED off
    if (inByte == '1')
      digitalWrite(blue_led, HIGH); //turn LED on
  }
}

void setup() {
  pinMode(blue_led, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
  }
}

void loop() {
  console_routine();
}
