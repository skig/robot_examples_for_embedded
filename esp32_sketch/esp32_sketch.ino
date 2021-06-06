/**
 * The sketch cotains two main parts:
 * - Serial console for controlling LED
 * - BLE GATT service
 */
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "0fc04e02-c8f4-4d51-aad6-35c5db47c27e"
#define CHARACTERISTIC_UUID "ece27bad-3d4b-4072-8494-76a551f0b6cc"

const uint8_t blue_led = 2; //LED pin on ESP32 DEVKIT V1 DOIT board
uint8_t led_status = '0';
uint8_t char_value = '0';

BLEServer* pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void console_routine(void) {
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == '0') {
      digitalWrite(blue_led, LOW); //turn LED off
      led_status = '0';
    }
    if (inByte == '1') {
      digitalWrite(blue_led, HIGH); //turn LED on
      led_status = '1';
    }
  }
}

void ble_connection_routine(void) {
    // notify changed value
    if (deviceConnected) {
        if (char_value != led_status) {
          char_value = led_status;          
          pCharacteristic->setValue((uint8_t*)&char_value, 1);
        }
        delay(3);
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }
}

void setup() {
  pinMode(blue_led, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
  }

  BLEDevice::init("ESP32_LED_STATUS");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  pService = pServer->createService(SERVICE_UUID);
  
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ);

  pCharacteristic->setValue((uint8_t*)&char_value, 1);
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
}

void loop() {
  console_routine();
  ble_connection_routine();
}
