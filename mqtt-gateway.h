#ifndef ESP_MQTT_GATEWAY_H
#define ESP_MQTT_GATEWAY_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "gateway.h"

class MqttGateway : public Gateway {
  public:
    void begin(WiFiClient& tcpClient, char* server, uint16_t port, char* clientId);

    bool send(Package& package);

  protected:
    PubSubClient mqttClient;
    char* clientId;

    void reconnect();
};

#endif // ESP_MQTT_GATEWAY_H

