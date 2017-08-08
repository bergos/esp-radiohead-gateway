#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <RH_RF69.h>
#include <RHReliableDatagram.h>

//#define USE_SOCKET_GATEWAY 1
#define USE_MQTT_GATEWAY 1

#ifdef USE_SOCKET_GATEWAY
  #include "socket-gateway.h"

  #define SOCKET_PORT 5000

  SocketGateway gateway(SOCKET_PORT);
#endif // USE_SOCKET_GATEWAY

#ifdef USE_MQTT_GATEWAY
  #include "mqtt-gateway.h"

  #define MQTT_SERVER "mqtt"
  #define MQTT_PORT 1883
  #define MQTT_CLIENT_ID "esp-radiohead-gateway"

  WiFiClient mqttClient;
  MqttGateway gateway;
#endif // USE_MQTT_GATEWAY

#define RADIO_NETWORK 0
#define RADIO_ADDRESS 1

RH_RF69 radio(15, 4);
RHReliableDatagram datagram(radio, RADIO_ADDRESS);

void setup() {
  Serial.begin(115200);

  Serial.println("start WiFiManager...");
  WiFiManager wifiManager;
  wifiManager.autoConnect("rfm69-bridge");
  Serial.println("...connected to WiFi");

#if USE_SOCKET_GATEWAY
  Serial.print("start SocketGateway...");
  gateway.begin();
  Serial.println("done");
#endif

#if USE_MQTT_GATEWAY
  Serial.print("init MqttGateway...");
  gateway.begin(mqttClient, MQTT_SERVER, MQTT_PORT, MQTT_CLIENT_ID);
  Serial.println("done");
#endif

  Serial.print("init radioManager...");
  if (!datagram.init()) {
    Serial.println("failed");
  } else {
    radio.setFrequency(868.0);
    // radio.setTxPower(20);
    Serial.println("done");
  }
}

void recv() {
  Package package;

  if(!gateway.recv(package)) {
    return;
  }

  Serial.print("send radio package to: 0x");
  Serial.print(package.destinationAddress, HEX);
  Serial.print("...");

  if (!datagram.sendtoWait(package.data, package.length - 4, package.destinationAddress)) {
    Serial.println("failed");
  } else {
    Serial.println("done");
  }
}

void send() {
  if (!datagram.available()) {
    return;
  }

  Package package;

  package.length = sizeof(package.data);

  if (!datagram.recvfromAck(package.data, &package.length, &package.sourceAddress, &package.destinationAddress)) {
    return;
  }

  package.sourceNetwork = RADIO_NETWORK;
  package.destinationNetwork = RADIO_NETWORK;

  Serial.print("received radio package from 0x");
  Serial.print(package.sourceAddress, HEX);
  Serial.print(" (");
  Serial.print(package.length);
  Serial.print(" bytes RSSI: ");
  Serial.print(radio.lastRssi());
  Serial.println(")");

  gateway.send(package);
}

void loop() {
  recv();
  send();
}

