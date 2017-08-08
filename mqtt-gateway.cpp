#include "mqtt-gateway.h"

void MqttGateway::begin(WiFiClient& tcpClient, char* server, uint16_t port, char* clientId) {
  mqttClient.setClient(tcpClient);
  mqttClient.setServer(server, port);

  this->clientId = clientId;

  reconnect();
}

void MqttGateway::reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("connect to MQTT server...");

    if (mqttClient.connect(clientId)) {
      Serial.println("connected to MQTT server");
    } else {
      Serial.print(".");
      delay(500);
    }
  }
}

bool MqttGateway::send(Package& package) {
  if (!mqttClient.connected()) {
    reconnect();
  }

  mqttClient.loop();

  String path = "/home/" + String(package.sourceNetwork) + "/" + String(package.sourceAddress);


  Serial.print("send MQTT message to: ");
  Serial.print(path);
  Serial.print("...");

  if(mqttClient.publish(path.c_str(), package.data, package.length)) {
    Serial.println("done");
  } else {
    Serial.println("failed");
  }

  return true;
}

