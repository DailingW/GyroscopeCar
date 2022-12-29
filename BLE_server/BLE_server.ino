/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define XCHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define YCHARACTERISTIC_UUID "bd2dbb6d-cf4c-4082-a827-a8b89edae723"
#define ZCHARACTERISTIC_UUID "34304421-acb3-4ff4-ae78-884032e9d92a"
#define RXD2 16
#define TXD2 17

//const int LED = A1;
BLECharacteristic *gyrox;
BLECharacteristic *gyroy;
BLECharacteristic *gyroz;


void setup() {
  //Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Starting BLE work!");

  BLEDevice::init("RC Car's ESP32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  gyrox = pService->createCharacteristic(XCHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  gyroy = pService->createCharacteristic(YCHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  gyroz = pService->createCharacteristic(ZCHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  gyrox->setValue("nothing");
  gyroy->setValue("nothing");
  gyroz->setValue("nothing");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  //pServer->startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial2.println("This Message Is Being Transmitted from ESP32");
  String d1 = gyrox->getValue().c_str();
  //Serial.println(d1);
  //Serial2.println(d1);
  String d2 = gyroy->getValue().c_str();
  //Serial.println(d2);
  //Serial2.println(d2);
  String d3 = gyroz->getValue().c_str();
  //Serial.println(d3);
  //Serial2.println(d3);
  String all = d1 + " " + d2;
  Serial2.println(all);
  //Serial.println("-----------------------");
  delay(1500);
}