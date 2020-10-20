// Светодиод подлкючен к 5 пину
// Датчик температуры ds18b20 к 2 пину

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid =  "ASUS";  // Имя вайфай точки доступа
const char *pass =  "pkrzk1963"; // Пароль от точки доступа

const char *mqtt_server = "broker.mqttdashboard.com"; // Имя сервера MQTT
const int mqtt_port = 1883; // Порт для подключения к серверу MQTT
const char *mqtt_user = ""; // Логи от сервер
const char *mqtt_pass = ""; // Пароль от сервера

#define BUFFER_SIZE 100

String device = "pult"; // Номер устройства (Должен быть уникальным)
String payload;


WiFiClient wclient;
PubSubClient client(wclient, mqtt_server, mqtt_port);

// Функция получения данных от сервера

void callback(const MQTT::Publish& pub){
  payload = pub.payload_string();
  Serial.println(payload);

}

void setup() {

  Serial.begin(115200);

}

void loop() {
  // подключаемся к wi-fi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  // подключаемся к MQTT серверу
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {

      Serial.println("Connecting to MQTT server");

      if (client.connect(MQTT::Connect(device).set_auth(mqtt_user, mqtt_pass)))
        {
          Serial.println("Connected to MQTT server");
          client.set_callback(callback);
          client.subscribe("alarm/d1");
          client.subscribe("alarm/d2");
        }

      else { Serial.println("Could not connect to MQTT server");}
    }

    if (client.connected()){client.loop();}



    }
}// конец основного цикла
