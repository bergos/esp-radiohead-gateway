#ifndef ESP_GATEWAY_H
#define ESP_GATEWAY_H

#include <Arduino.h>

struct Package {
  uint8_t sourceNetwork;
  uint8_t sourceAddress;
  uint8_t destinationNetwork;
  uint8_t destinationAddress;
  uint8_t data[255];
  uint8_t length;
};

class Gateway {
  public:
    virtual bool recv(Package& package) {
      return false;
    }

    virtual bool send(Package& package) {
      return false;
    }
};

#endif // ESP_RADIOHEAD_GATEWAY_H

