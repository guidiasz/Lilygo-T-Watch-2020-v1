#pragma mark - Depend ESP8266Audio and ESP8266_Spiram libraries
/*
cd ~/Arduino/libraries
git clone https://github.com/earlephilhower/ESP8266Audio
git clone https://github.com/Gianbacchio/ESP8266_Spiram
*/

#include <HTTPClient.h>  //Remove Audio Lib error

// --- Audio ---
#include "AudioFileSourceID3.h"
#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "config.h"
#include "susto.h"

AudioGeneratorMP3 *mp3;
AudioFileSourcePROGMEM *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

// --- Image ---
#include "exorcista.h"

TTGOClass *ttgo;

void playAudio() {
  //! Turn on the audio power
  ttgo->enableLDO3();

  file = new AudioFileSourcePROGMEM(susto, sizeof(susto));
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

void displayImage() {
  ttgo->tft->setSwapBytes(true);
  ttgo->tft->pushImage(0, 0, 240, 240, exorcista);
}

void setup() {
  Serial.begin(115200);

  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();

  playAudio();
  displayImage();
  ttgo->motor_begin(); //pelo que entendi, liga o motor de vibração
}

void loop() {
  if (mp3->isRunning()) {
    ttgo->motor->onec(); //comando que faz o relogio vibrar
    if (!mp3->loop()) {
      mp3->stop();
      // O código que for colocado a seguir só será executado quando terminar a reprodução do áudio
      ttgo->tft->setTextSize(2);
      ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
      ttgo->tft->setCursor(16, 16);
      ttgo->tft->println("Feliz Halloween :D");
    }
  } else {  // o código abaixo é executado somente se nenhum áudio está sendo executado
    delay(500);
  }
}
