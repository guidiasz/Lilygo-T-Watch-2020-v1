# Reconhecer a aproximação de dispositivos BLE

**Passo 1:** Adicione as seguintes bibliotecas e variáveis:
```cpp
// --- Bluetooth ---
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>

int scanTime = 0;  // Duracao do scan em segundos. Se 0, o scan eh "infinito"
BLEScan *pBLEScan;

const int RSSI_LEVEL = -45; //Nivel de distância entre os dispositivos desejado para emitir um alerta. Quanto perto de 0, mais próximo o dispositivo está
const String AUTHORIZED_DEVICE = "f3:f4:1d:8d:89:6e"; //dispositivo que vamos verificar
```

**Passo 2:** Adicione também a função `scanBLE()`:
```cpp
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
```

Para entender a diferença entre `setInterval` e `setWindow`, consulte a imagem `diferenca-entre-interval-e-window.jpg`

**Passo 3:** Adicione a classe:
```cpp
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String foundDevice = advertisedDevice.getAddress().toString().c_str();
    if (foundDevice == AUTHORIZED_DEVICE) { //checa se o dispositivo desejado esta na lista de dispositivos visiveis
      if (advertisedDevice.getRSSI() >= RSSI_LEVEL) { //verifica se o dispositivo esta proximo o suficiente para um alerta
        ttgo->tft->setTextSize(3);
        ttgo->tft->setCursor(5, 40);
        ttgo->tft->print("Guilherme se aproximou do relogio!");
      }
  }
};
```

**Passo 4:** Por fim, a função `setup()` ficará assim: 
```cpp
TTGOClass *ttgo;

// --- Setup ---
void setup() {
  Serial.begin(115200);

  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();

  ttgo->tft->setTextSize(2);
  ttgo->tft->fillScreen(TFT_BLACK);
  ttgo->tft->setCursor(5, 40);
  ttgo->tft->print("Pesquisando...");

  scanBLE();
}
```

Resultado esperado:
[![Demo](https://res.cloudinary.com/marcomontalbano/image/upload/v1670818123/video_to_markdown/images/video--6474385018c639c85c46937984f64b7a-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://raw.githubusercontent.com/guidiasz/Lilygo-T-Watch-2020-v1/main/apps-individuais/detectorBLE/demo.mp4 "Demo")
