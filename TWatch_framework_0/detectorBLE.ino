#pragma mark - Depend ESP8266Audio and ESP8266_Spiram libraries
/*
cd ~/Arduino/libraries
git clone https://github.com/earlephilhower/ESP8266Audio
git clone https://github.com/Gianbacchio/ESP8266_Spiram
*/

#include <HTTPClient.h>  //Remove Audio Lib error

// --- Audio ---
#include "./appDetectorBLE/config.h"
#include "./appDetectorBLE/image.h"
#include "AudioFileSourceID3.h"
#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

AudioGeneratorMP3 *audio;
bool play = false;

// --- Bluetooth ---
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>

int scanTime = 0;  // Duracao do scan em segundos. Se 0, o scan eh "infinito"
BLEScan *pBLEScan;

const int RSSI_LEVEL = -45;
const String AUTHORIZED_DEVICE = "f3:f4:1d:8d:89:6e";

// --- Setup ---
void detectorBLE() {
  int16_t x, y;
  Serial.begin(115200);

  ttgo->tft->setTextSize(2);
  ttgo->tft->fillScreen(TFT_BLACK);
  ttgo->tft->setCursor(5, 40);
  ttgo->tft->print("Pesquisando...");

  scanBLE();
  while (!ttgo->getTouch(x, y)) {  // se tocar no display, o codigo para de ser executado
    if (play) {
      soundLoop();
    }
  }
  while (ttgo->getTouch(x, y)) {
  }                                  // Wait for release to return to the clock
  ttgo->tft->fillScreen(TFT_BLACK);  // Clear screen
}

// -- Funções Auxiliares ---
// TODO: fazer versão com celular
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  bool cleanScreen = false;
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String foundDevice = advertisedDevice.getAddress().toString().c_str();
    if (foundDevice == AUTHORIZED_DEVICE) {
      if (advertisedDevice.getRSSI() >= RSSI_LEVEL) {
        ttgo->tft->fillScreen(TFT_BLACK);
        ttgo->tft->setTextSize(3);
        ttgo->tft->setCursor(5, 40);
        ttgo->tft->print("Guilherme se aproximou do relogio!");
        if (!play) {
          play = true;
          playMusic();
          pBLEScan->stop();
        }
      }
    }

    // Serial.printf("Advertised Device: %s \n", advertisedDevice.getAddress().toString().c_str());
  }
};

// --- Scan Bluetooth LE ---
void scanBLE() {
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();  // create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  // active scan uses more power, but get results faster
  pBLEScan->setInterval(500);
  pBLEScan->setWindow(500);  // less or equal setInterval value
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();  // delete results fromBLEScan buffer to release memory
}

void playMusic() {
  AudioFileSourcePROGMEM *file;
  AudioOutputI2S *out;
  AudioFileSourceID3 *id3;

  //! Turn on the audio power
  ttgo->enableLDO3();

  file = new AudioFileSourcePROGMEM(music, sizeof(music));
  id3 = new AudioFileSourceID3(file);

#if defined(STANDARD_BACKPLANE)
  out = new AudioOutputI2S(0, 1);
#elif defined(EXTERNAL_DAC_BACKPLANE)
  out = new AudioOutputI2S();
  // External DAC decoding
  out->SetPinout(TWATCH_DAC_IIS_BCK, TWATCH_DAC_IIS_WS, TWATCH_DAC_IIS_DOUT);
#endif
  audio = new AudioGeneratorMP3();
  audio->begin(id3, out);
}

void soundLoop() {
  if (audio->isRunning() && !audio->loop()) {
    audio->stop();
    Serial.printf("MP3 done\n");
    play = !play;
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setCursor(40, 40);
    ttgo->tft->setTextSize(2);
    ttgo->tft->print("Pesquisando...");
    pBLEScan->start(scanTime, false);
  }
}
