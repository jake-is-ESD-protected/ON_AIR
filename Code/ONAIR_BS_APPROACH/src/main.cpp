#include <Arduino.h>
#include "polling.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "6f3265cf-fff1-4324-bd01-f24b83023b29"
#define CHARACTERISTIC_UUID "31086795-5d79-430a-8a47-6ba4ad441d0b"

LiquidCrystal_I2C lcd(0x27, 16, 2);

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;

void setup()
{
  
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  pinMode(LED_ARRAY, OUTPUT);
  pinMode(BUTTON_LED, OUTPUT);
  pinMode(BUTTON, INPUT);

  Serial.println("Starting BLE Server!");

  BLEDevice::init("ESP32-BLE-Server");
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue("Hello, World!");
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

}

void loop() 
{
  std::string value = pCharacteristic->getValue();
  Serial.print("The new characteristic value is: ");
  Serial.println(value.c_str());
  lcd.printf(value.c_str());
  delay(2000);
}