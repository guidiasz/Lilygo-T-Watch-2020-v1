#include "config.h"

TTGOClass* ttgo;

// --- ACCELEROMETER ---
Accel acc;
int16_t xpos, ypos;

// --- WIFI ---
#include <WiFi.h>

// Please input the SSID and password of WiFi
const char* ssid = "Lula 2022";
const char* password = "m3rc!03ut34m0";
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

// --- MQTT ---
#include <PubSubClient.h>
PubSubClient client(esp32Client);
const char* mqtt_Broker = "mqtt.eclipseprojects.io";
const char* topic[2] = {"acelerometroTWatch/valorX", "acelerometroTWatch/valorY"};

const char* mqtt_ClientID = "esp32-01";

void MQTTConnect() {
  ttgo->tft->print("\nConectando ao MQTT...\n");
  while (!client.connected()) {
    client.connect(mqtt_ClientID);
    ttgo->tft->print(".");
  }
  ttgo->tft->print("\nSucesso!\n");
}

void publishOnTopic(int topicIndex, int value) {
  client.publish(topic[topicIndex], String(value).c_str(), true);
}

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

int cont = 0;
void loop() {
  ttgo->bma->getAccel(acc);
  xpos = acc.x;
  ypos = acc.y;
  ttgo->tft->fillCircle(xpos / 10 + 119, ypos / 10 + 119, 10, TFT_RED);  // draw dot
  delay(100);
  ttgo->tft->fillCircle(xpos / 10 + 119, ypos / 10 + 119, 10, TFT_BLACK);  // erase previous dot

  ttgo->tft->fillRect(80, 160, 200, 200, TFT_BLACK);
  ttgo->tft->setCursor(80, 160);
  ttgo->tft->print("X: ");
  ttgo->tft->print(xpos);
  ttgo->tft->setCursor(80, 190);
  ttgo->tft->print("Y: ");
  ttgo->tft->print(ypos);
  if (cont == 50) {
    publishOnTopic(0, xpos);
    publishOnTopic(1, ypos);
    cont = 0;
  }

  cont++;
}
