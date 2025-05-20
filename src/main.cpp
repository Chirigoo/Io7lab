#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Chirigoo";
const char* password = "kkkkkkkk";

const char* mqtt_server = "iot101.io7lab.com";
const char* topic_sub = "iot3/lamp1/cmd/power/fmt/json";
const char* topic_pub = "iot3/lamp1/evt/status/fmt/json";

WiFiClient espClient;
PubSubClient client(espClient);

const int relayPin = 15;

void setup_wifi() {
  delay(100);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (message.indexOf("on") >= 0) {
    digitalWrite(relayPin, HIGH);
    client.publish(topic_pub, "{\"power\":\"on\"}");
  } else if (message.indexOf("off") >= 0) {
    digitalWrite(relayPin, LOW);
    client.publish(topic_pub, "{\"power\":\"off\"}");
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("lamp1")) {
      client.subscribe(topic_sub);
    } else {
      delay(1000);
    }
  }
}

void setup() {
  pinMode(relayPin, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
