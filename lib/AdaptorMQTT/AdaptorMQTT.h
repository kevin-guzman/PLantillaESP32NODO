#ifndef mqqt_h
#define mqqt_h
#include <Arduino.h>
#include <PubSubClient.h>

class AdaptorMQTT{
  public:
    AdaptorMQTT(const char* MQTT_SERVER, int MQTT_PORT, String clientId);
    virtual ~AdaptorMQTT();
    void loop();
    void setTopics(String _topics);
    void send(const char* topic, String message);
    void connect(void (*callback)(char* _topic, byte* _payload, unsigned int length));
  private:
    PubSubClient _client;
    String parsePayload(byte* payload, unsigned int length);
    void reconnect();
    String _clientId;
    const char* _MQTT_SERVER;
    int _MQTT_PORT;
    String _topics;
};

#endif