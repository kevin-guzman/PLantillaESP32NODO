#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "AdaptorMQTT.h"

WiFiClient espClient;
PubSubClient client(espClient);

AdaptorMQTT::AdaptorMQTT(const char* MQTT_SERVER, int MQTT_PORT, String clientId){
  _clientId = clientId;
  _MQTT_SERVER = MQTT_SERVER;
  _MQTT_PORT = MQTT_PORT;
  _client = client;
}

AdaptorMQTT::~AdaptorMQTT(){}

String parsePayload(byte* payload, unsigned int length){
  String value;
  for (int i = 0; i < length; i++) {
    value+=((char)payload[i]);
  }
  return value;
}

void AdaptorMQTT::connect(void (*callback)(char* _topic, byte* _payload, unsigned int length)){
  _client.setServer(_MQTT_SERVER, _MQTT_PORT);
  _client.setCallback(callback);
}

void AdaptorMQTT::setTopics(String topics){
  _topics=topics;
}

void AdaptorMQTT::loop(){
  if (!_client.connected()) {
    this->reconnect();
  }else{
    _client.loop();
  }
}

void AdaptorMQTT::reconnect() {
  while (!_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (_client.connect(_clientId.c_str())){
      Serial.println("connected");
      String topic="";
      int topicsLength = _topics.length();
      for (unsigned int i = 0; i < topicsLength +1; i++) {
        if(_topics.charAt(i) != ',' && _topics.charAt(i) != '\0'){
          topic+=_topics[i];
        }else{
          _client.subscribe(topic.c_str());
          topic="";
        }
        delay(10);
      }
    } else {
      Serial.print("failed, reconnection--> ");
      Serial.print(_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void AdaptorMQTT::send(const char* topic, String message){
  if(_client.connected()){
    if (_client.connect(_clientId.c_str())) {
      Serial.println("sending message ->");
      const int json_str_len = message.length();
      const int cut = 100;
      if (json_str_len > cut){
        _client.beginPublish(topic, json_str_len, true);
        int count = json_str_len / cut;
        for (int i = 0; i < (count-1); i++){
          const String cuttedString = message.substring(i * cut, (i * cut + cut));
          _client.print(cuttedString);
        }
        const String lastString = message.substring(cut * (count - 1));
        _client.print(lastString);
        _client.endPublish();
      }
      else{
        _client.publish(topic, message.c_str());
      }
      Serial.println("sended message successfuly ---->");
    }
  }
}