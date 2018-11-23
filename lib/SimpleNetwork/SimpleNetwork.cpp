#include <SimpleNetwork.h>

bool initializeNetwork(){
  Ethernet.begin(mac, listenAddress);
}
