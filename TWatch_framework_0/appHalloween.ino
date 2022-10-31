#pragma mark - Depend ESP8266Audio and ESP8266_Spiram libraries
/*
cd ~/Arduino/libraries
git clone https://github.com/earlephilhower/ESP8266Audio
git clone https://github.com/Gianbacchio/ESP8266_Spiram
*/

#include <HTTPClient.h>  //Remove Audio Lib error

// --- Audio ---
#include "./appHalloween/config.h"
#include "./appHalloween/susto.h"
#include "AudioFileSourceID3.h"
#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

AudioGeneratorMP3 *mp3;

// --- Image ---
#include "./appHalloween/exorcista.h"

// --- Funcoes assincronas ---
#include <AsyncTimer.h>  //https://github.com/Aasim-A/AsyncTimer

void playAudio() {
  AudioFileSourcePROGMEM *file;
  AudioOutputI2S *out;
  AudioFileSourceID3 *id3;
  //! Turn on the audio power
  ttgo->enableLDO3();

  file = new AudioFileSourcePROGMEM(image, sizeof(image));
  id3 = new AudioFileSourceID3(file);

#if defined(STANDARD_BACKPLANE)
  out = new AudioOutputI2S(0, 1);
#elif defined(EXTERNAL_DAC_BACKPLANE)
  out = new AudioOutputI2S();
  // External DAC decoding
  out->SetPinout(TWATCH_DAC_IIS_BCK, TWATCH_DAC_IIS_WS, TWATCH_DAC_IIS_DOUT);
#endif
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);
}

void vibrate() {
  ttgo->motor->onec();
}
void displayImage() {
  ttgo->tft->setSwapBytes(true);
  ttgo->tft->pushImage(0, 0, 240, 240, exorcista);
}

void appHalloween() {
  AsyncTimer t;

  int16_t x, y;

  Serial.begin(115200);

  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();

  playAudio();
  displayImage();

  /*vibracao*/
  ttgo->motor_begin();
  for (int delayTime = 200; delayTime <= 1000; delayTime += 200) {
    t.setTimeout(vibrate, delayTime);  // chama a funcao vibrate de forma assincrona com timer em milessegundos
  }

  while (!ttgo->getTouch(x, y)) {
    t.handle();
    if (mp3->isRunning()) {
      if (!mp3->loop()) mp3->stop();
    } else {
      ttgo->tft->setTextSize(2);
      ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
      ttgo->tft->setCursor(16, 16);
      ttgo->tft->println("Feliz Halloween :D");
    }
  }
  while (ttgo->getTouch(x, y)) {
  }  // Wait for release to return to the clock
  ttgo->tft->setTextSize(1);
  ttgo->tft->fillScreen(TFT_BLACK);  // Clear screen
}
