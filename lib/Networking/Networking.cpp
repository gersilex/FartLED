#include <Networking.h>

Networking::Networking(networkConfig config){
  this->config = config;
  this->alreadyConnected = false;
}

const char* Networking::read(EthernetServer server){
  // wait for a new client:
  EthernetClient client = server.available();
  if (client) {
    if (!this->alreadyConnected) {
      // clear out the input buffer:
      client.flush();
      Serial.println("We have a new client");
      client.println("Hello, client!");
      this->alreadyConnected = true;
    }

    int contentsize = client.available();

    //Serial.println(client.available());

    char resulttest[61];
    char* result = (char*)malloc(contentsize);


    client.read();

    return result;
  }

  else
  {
    delay(1000);
    return ".";
  }
}

void Networking::begin(){
  this->config.server = EthernetServer(this->config.listenPort);
  Ethernet.begin(this->config.mac, this->config.listenAddress);
}
