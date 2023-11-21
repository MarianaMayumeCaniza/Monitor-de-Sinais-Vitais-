#include <Wire.h>
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "";
const char *password = "";
const char *mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char *mqtt_topic = "Batimentos";

#define RXp2 16
#define TXp2 17

WiFiClient espClient;
PubSubClient client(espClient);
  int batimentos = 0;  // Substituir pelo valor real dos batimentos
  int spo2 = 0;  
  float temperatura = 0.0;


void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  setupWiFi();
  setupMQTT();
}

void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando à rede WiFi");
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Conectado à rede WiFi!");
  } else {
    Serial.println("Falha ao conectar à rede WiFi!");
  }
}

void setupMQTT() {
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  Serial.println(Serial2.readString());
  String data = Serial2.readString();
  Serial.print(data);
  // Extraindo os valores de BPM, SPO2 e temperatura da string
  sscanf(data.c_str(), "BPM: %d\nSPO2:%d\ntemperatura: %f", &batimentos, &spo2, &temperatura);
    
    // Agora você tem os valores armazenados nas variáveis bpm, spo2 e temperatura
    // Faça o que quiser com esses valores
  Serial.print("batimento: ");
  Serial.println(batimentos);
  Serial.print("sat: ");
  Serial.println(spo2);
  Serial.print("temp: ");
  Serial.println(temperatura);

  if (!client.connected()) {
    reconnectMQTT();
  }
  publishMQTTData();
  client.loop();
  delay(1000);
}



void publishMQTTData() {
  if (!client.connected()) {
    reconnectMQTT();
  }

  if (client.connected()) {
    String bpmTopic = String(mqtt_topic) + "/batimentos";
    String spo2Topic = String(mqtt_topic) + "/spo2";
    String temperatureTopic = String(mqtt_topic) + "/temperatura";

    String bpmString = String(batimentos);
    String spo2String = String(spo2);
    String temperaturaString = String(temperatura);

    client.publish(bpmTopic.c_str(), bpmString.c_str());
    client.publish(spo2Topic.c_str(), spo2String.c_str());
    client.publish(temperatureTopic.c_str(), temperaturaString.c_str());
    Serial.println("Enviado para MQTT: Batimentos=" + bpmString + ", SpO2=" + spo2String + ", Temperatura=" + temperatura);
  }
}

void reconnectMQTT() {
  int attempts = 0;
  while (!client.connected() && attempts < 5) {
    Serial.print("Tentando se reconectar ao MQTT Broker...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
    attempts++;
  }
}

