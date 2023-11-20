#include <Wire.h>
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "Caniza";
const char *password = "37650807";
const char *mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char *mqtt_topic = "Batimentos";

WiFiClient espClient;
PubSubClient client(espClient);
 int batimentos = 75;  // Substituir pelo valor real dos batimentos
  int spo2 = 98;  
void setup() {
  Serial.begin(9600);
  Wire.begin(9, 21, 22); // Pinos SDA (21) e SCL (22) no ESP32
  Wire.onReceive(receiveEvent);
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
  if (!client.connected()) {
    reconnectMQTT();
  }
  Wire.onReceive(receiveEvent);
  publishMQTTData();
  client.loop();
  delay(1000);
}

void receiveEvent(int howMany) {
  Serial.println("Recebendo dados via I2C:");

  String receivedData = "";
  while (Wire.available() > 0) {
    char c = Wire.read();
    receivedData += c;
  }

  Serial.println(receivedData);

  // Agora, você precisa analisar os dados recebidos e atualizar as variáveis
  sscanf(receivedData.c_str(), "BPM:%d, SPO2:%d", &batimentos, &spo2);

  // Exiba os dados atualizados no Serial Monitor
  Serial.print("Batimentos atualizados: ");
  Serial.print(batimentos);
  Serial.print(", SpO2 atualizado: ");
  Serial.println(spo2);
}

void publishMQTTData() {
  if (!client.connected()) {
    reconnectMQTT();
  }

  if (client.connected()) {
    String bpmTopic = String(mqtt_topic) + "/batimentos";
    String spo2Topic = String(mqtt_topic) + "/spo2";

    String bpmString = String(batimentos);
    String spo2String = String(spo2);

    client.publish(bpmTopic.c_str(), bpmString.c_str());
    client.publish(spo2Topic.c_str(), spo2String.c_str());
    Serial.println("Enviado para MQTT: Batimentos=" + bpmString + ", SpO2=" + spo2String);
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
