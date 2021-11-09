#ifndef ws_h
#define ws_h
#include <Arduino.h>
#include <ArduinoWebsockets.h>
using namespace websockets;

class AdaptorWSClass
{
private:
  /* data */
  void reconnect();
  char * _password, _user;
  String _reconnectTopic, _reconnectMessage;
  String ip;
  void onConnect();

public:
  AdaptorWSClass(String host, int port);
  void init(String host, int port, String connectTopic, String connectionMessage);
  ~AdaptorWSClass();
  void send(String topic,String message);
  void loop();
  void auth();
  void onMessage(void (*callback)(WebsocketsMessage msg));
};

#endif