#ifndef ESP_SOCKET_GATEWAY_H
#define ESP_SOCKET_GATEWAY_H

#include <ESP8266WiFi.h>

#include "gateway.h"

class SocketGateway :public Gateway {
  public:
    SocketGateway(uint16_t port);
  
    void begin();

    virtual bool recv(Package& package);

    virtual bool send(Package& package);

  protected:
    WiFiServer tcpServer;
    WiFiClient tcpClient;

    void connect();
};

#endif // ESP_SOCKET_GATEWAY_H
