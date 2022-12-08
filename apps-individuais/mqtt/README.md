### MQTT

Conecte-se ao wifi:
```cpp
// --- WIFI ---
#include <WiFi.h>

// Please input the SSID and password of WiFi
const char* ssid     = "meu-wifi";
const char* password = "minha-senha";
WiFiClient esp32Client;

void wifiConnect() {
  ttgo->tft->print("Conectando ao wifi...");
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    ttgo->tft->print(".");
  }
  ttgo->tft->print("\nWiFi connected\n");
  ttgo->tft->print("IP address: \n");
  ttgo->tft->print(WiFi.localIP());
}
```

No Arduino IDE, instale a biblioteca `PubSubClient`, de Nick O'leary.Em seguida, adcione o código a seguir:
```cpp
// --- MQTT ---
#include <PubSubClient.h>
PubSubClient client(esp32Client);
const char* mqtt_Broker = "mqtt.eclipseprojects.io";
const char* topic[2] = {"acelerometroTWatch/valorX","acelerometroTWatch/valorY"};

const char* mqtt_ClientID = "esp32-01";
```
Nesse código estamos definindo o [broker](https://iot4beginners.com/top-15-open-source-public-free-brokers-of-mqtt/ "broker"), o tópico no qual vamos usar e o ID. O ID deve ser único por tópico.

Agora, vamos criar a função que se conecta no MQTT:
```cpp
void MQTTConnect() {
  ttgo->tft->print("\nConectando ao MQTT...\n");
  while (!client.connected()) {
    client.connect(mqtt_ClientID);
    ttgo->tft->print(".");
  }
  ttgo->tft->print("\nSucesso!\n");
}
```

E a função que publica uma mensagem no tópico:

```cpp
void publishOnTopic(int topicIndex, int value){
  client.publish(topic[topicIndex], String(value).c_str(), true);
}
```

No `setup()`,  a chamada das funções   `wifiConnect();`,  `client.setServer(mqtt_Broker, 1883);` e  `MQTTConnect();`  fazem o MQTT funcionar.
```cpp
void setup() {
  Serial.begin(115200);
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();
  ttgo->bma->begin();
  ttgo->bma->enableAccel();
  ttgo->tft->fillScreen(TFT_BLACK);

  wifiConnect();
  client.setServer(mqtt_Broker, 1883);
  MQTTConnect();
  delay(500);
  ttgo->tft->fillScreen(TFT_BLACK);
}
```

### Acelerômetro
No exemplo em questão, vamos compartilhar os dados do acelerômetro via MQTT.  Vamos incluir o seguinte código: 
```cpp
// --- ACCELEROMETER ---
Accel acc;
int16_t xpos, ypos;

```
Em `setup()`, os métodos  `ttgo->bma->begin();` e `ttgo->bma->enableAccel();` fazem o acelerômetro funcionar.

Em `loop()`, o código a seguir guarda os valores de x e y do acelerômetro em `xpos` e `ypos`:
```cpp
    ttgo->bma->getAccel(acc);
    xpos = acc.x;
    ypos = acc.y;
```

A cada 5 segundos, o código a seguir publica os valores dos seus respectivos tópicos:
```cpp
if (cont == 50) {
    publishOnTopic(0, xpos);
    publishOnTopic(1, ypos);
    cont = 0;
}
```

### Visualizando os valores com o Google Collab
Basta executar o código [desse link](https://colab.research.google.com/drive/1XzNggtRncSO-IrqsvoSr3nukkpuwUtp1?usp=sharing "desse link") e a partir daí você vai conseguir visualizar os dados recebidos do relógio via MQTT.