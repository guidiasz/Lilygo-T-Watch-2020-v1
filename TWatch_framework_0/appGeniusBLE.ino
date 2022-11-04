#include <stdlib.h>
#include <time.h>

/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "config.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyCallbacksBLE : public BLECharacteristicCallbacks {
 public:
  bool *moved;
  char *myMovements;
  int *cont;
  MyCallbacksBLE(bool &x, char y[], int &z) {
    moved = &x;
    myMovements = y;
    cont = &z;
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      for (int i = 0; i < value.length(); i++)
        Serial.print(value[i]);
      myMovements[*cont] = value[0];
      *moved = true;
    }
  }
};

void highlightBLE(char side) {
  if (side == 'u') {
    ttgo->tft->fillCircle(120, 90, 15, TFT_WHITE);
    delay(500);
    ttgo->tft->fillCircle(120, 90, 15, TFT_GREEN);

  } else if (side == 'r') {
    ttgo->tft->fillCircle(150, 120, 15, TFT_WHITE);
    delay(500);
    ttgo->tft->fillCircle(150, 120, 15, TFT_YELLOW);

  } else if (side == 'l') {
    ttgo->tft->fillCircle(90, 120, 15, TFT_WHITE);
    delay(500);
    ttgo->tft->fillCircle(90, 120, 15, TFT_RED);

  } else if (side == 'd') {
    ttgo->tft->fillCircle(120, 150, 15, TFT_WHITE);
    delay(500);
    ttgo->tft->fillCircle(120, 150, 15, TFT_BLUE);
  }
}

void fillSequenceBLE(char sequence[], int size) {
  char sides[4] = {'u', 'd', 'l', 'r'};
  for (int i = 0; i < size; i++) {
    sequence[i] = sides[rand() % 4];
    while (sequence[i - 1] == sequence[i]) sequence[i] = sides[rand() % 4];
  }
}

void appGeniusBLE() {
  srand(time(NULL));
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();

  ttgo->bma->begin();
  ttgo->bma->enableAccel();
  ttgo->tft->fillScreen(TFT_BLACK);
  int16_t x, y;

  char sequence[50];
  char myMovements[50];
  int cont = 0;
  int round = 1;
  bool moved = false;
  Accel acc;

  ttgo->tft->fillCircle(120, 90, 15, TFT_GREEN);
  ttgo->tft->fillCircle(90, 120, 15, TFT_RED);
  ttgo->tft->fillCircle(150, 120, 15, TFT_YELLOW);
  ttgo->tft->fillCircle(120, 150, 15, TFT_BLUE);
  fillSequenceBLE(sequence, 50);

  Serial.begin(115200);

  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setCallbacks(new MyCallbacksBLE(moved, myMovements, cont));

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  while (!ttgo->getTouch(x, y)) {  // Wait for touch to exit

    if (round - 1 == cont) {
      for (int i = 0; i < round; i++) {
        delay(200);
        highlightBLE(sequence[i]);
      }
      cont = 0;
      round++;
    }

    if (moved) {
      moved = false;
      highlightBLE(myMovements[cont]);
      if (myMovements[cont] != sequence[cont]) {
        ttgo->tft->setCursor(80, 190);
        ttgo->tft->print("Você perdeu!");
        delay(4000);
        break;
      }
      cont++;
    }
  }

  while (ttgo->getTouch(x, y)) {
  }  // Wait for release to return to the clock

  ttgo->tft->fillScreen(TFT_BLACK);  // Clear screen
}
