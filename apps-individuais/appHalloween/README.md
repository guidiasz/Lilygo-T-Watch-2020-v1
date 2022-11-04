# Reproduzir audio, imagem e vibração no relógio
### Audio
**Passo 1:** Para conseguir reproduzir som, é necessário usar o seguinte config.h
```cpp
// => Hardware select
// #define LILYGO_WATCH_2019_WITH_TOUCH     // To use T-Watch2019 with touchscreen, please uncomment this line
// #define LILYGO_WATCH_2019_NO_TOUCH       // To use T-Watch2019 Not touchscreen , please uncomment this line
// #define LILYGO_WATCH_BLOCK               // To use T-Watch Block , please uncomment this line
// #define LILYGO_WATCH_BLOCK_V1            // To use T-Watch Block V1 , please uncomment this line
#define LILYGO_WATCH_2020_V1  // To use T-Watch2020 V1 , please uncomment this line
// #define LILYGO_WATCH_2020_V3             // To use T-Watch2020 V3, please uncomment this line

// Except T-Watch2020, other versions need to be selected according to the actual situation
#if !defined(LILYGO_WATCH_2020_V1) && !defined(LILYGO_WATCH_2020_V3)

// T-Watch comes with the default backplane, it uses internal DAC
#define STANDARD_BACKPLANE

// Such as MAX98357A, PCM5102 external DAC backplane
// #define EXTERNAL_DAC_BACKPLANE

#else
// T-Watch2020 uses external DAC
#undef STANDARD_BACKPLANE
#define EXTERNAL_DAC_BACKPLANE

#endif

#include <LilyGoWatch.h>
```
**Passo 2:**  Em `appHaloween.ino`, inclua as seguintes bibliotecas e variáveis. Pode ser necessário a instalação de algumas bibliotecas no Arduino IDE para o código funcionar. 
```cpp
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
```
**Passo 3:** Adicione também a função `playAudio()`:
```cpp
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
```

Note que nesse bloco de código há uma variável chamada `susto` sendo utilizada. Essa variável é um vetor que contêm o áudio a ser reproduzido e foi declarada no arquivo `susto.h`. Para alterar o áudio do vetor, basta escolher o arquivo mp3, convertê-lo com o site http://tomeko.net/online_tools/file_to_hex.php?lang=en e então substituir o conteúdo do vetor.

**Passo 4:** O código a seguir precisa ser executado em loop, caso contrário o áudio não será executado corretamente. 
```cpp
if (mp3->isRunning()) { 
    if (!mp3->loop()) {
      mp3->stop();
      // O código que for colocado a seguir só será executado quando terminar a reprodução do áudio
      ttgo->tft->println("O audio terminou!");
    }
 }
 ```
 ### Imagem
 **Passo 1:** Converta a imagem desejada nesse site. Altere o formato do arquivo gerado de `.c` para `.h`.
 
**Observação:** Ao que parece o display do relógio possui a resolução 240x240. Tenha isso em mente ao converter imagens.

**Passo 2:** No arquivo `appHalloween.ino`, inclua a imagem convertida
```cpp
// --- Image ---
#include "exorcista.h"
```

**Passo 3:** Inclua também a função `displayImage()`
```cpp
void displayImage() {
  ttgo->tft->setSwapBytes(true);
  ttgo->tft->pushImage(0, 0, 240, 240, exorcista);
}
```
**Observação:** Tenha em mente que os valores passados para a função pushImage significam: `pushImage(x, y, imageWidth, imageHeight, imageArray);`.

### Vibração

**Passo 1:** Os 2 comandos necessários para executar uma vibração são:
```cpp
ttgo->motor_begin(); //pelo que entendi, liga o motor de vibração
ttgo->motor->onec(); //comando que faz o relogio vibrar
```

**Passo 2:** Adicione `ttgo->motor_begin();` na função `setup()`.

**Passo 3:** Como queremos que o relogio fique vibrando enquanto o áudio é reproduzido, nossa função `loop()` ficará da seguinte forma:
```cpp
void loop() {
  if (mp3->isRunning()) {
    ttgo->motor->onec(); //comando que faz o relogio vibrar
    if (!mp3->loop()) {
      mp3->stop();
      // O código que for colocado a seguir só será executado quando terminar a reprodução do áudio
    }
  }
}
```
