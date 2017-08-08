#include "socket-gateway.h"

SocketGateway::SocketGateway(uint16_t port) : tcpServer(port) {}

void SocketGateway::begin() {
  tcpServer.begin();
}

bool SocketGateway::recv(Package& package) {
  connect();

  if (!tcpClient || !tcpClient.connected()) {
    return false;
  }

  package.length = tcpClient.available();

  if (!package.length) {
    return false;
  }

  if (package.length > sizeof(package) - 4) {
    Serial.print("received too large TCP package from: ");
    Serial.println(tcpClient.remoteIP());

    while(tcpClient.available()) {
      tcpClient.read();
    }

    return false;
  }

  tcpClient.read((unsigned char*)&package, package.length);
  Serial.print("received TCP package from: ");
  Serial.print(tcpClient.remoteIP());
  Serial.print(" (");
  Serial.print(package.length);
  Serial.println(" bytes)");

  return true;
}

bool SocketGateway::send(Package& package) {
  if (tcpClient) {
    Serial.print("send TCP package to: ");
    Serial.print(tcpClient.remoteIP());
    Serial.print("...");
    tcpClient.write((char*)&package, package.length + 4);
    Serial.println("done");
  }
}

void SocketGateway::connect() {
  if (tcpClient.connected()) {
    return;
  }

  tcpClient = tcpServer.available();

  if (tcpClient) {
    Serial.print("client connected: ");
    Serial.println(tcpClient.remoteIP());
  }
}
