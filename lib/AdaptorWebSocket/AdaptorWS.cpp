#include <Arduino.h>
#include <WiFi.h>
#include <Arduino_JSON.h>
#include <ArduinoWebsockets.h>

#include "AdaptorWS.h"

using namespace websockets;

WebsocketsClient webSocketClient;

AdaptorWSClass::AdaptorWSClass(String host, int port){}

void onEventsCallback(WebsocketsEvent event, String data){
  if (event == WebsocketsEvent::ConnectionOpened){
    Serial.println("Connnection Opened");
  }
  else if (event == WebsocketsEvent::ConnectionClosed){
    Serial.println("Connnection Closed");
  }
  else if (event == WebsocketsEvent::GotPing){
    Serial.println("Got a Ping!");
  }
  else if (event == WebsocketsEvent::GotPong){
    Serial.println("Got a Pong!");
  }
}

void AdaptorWSClass::init(String _host, int _port, String connectTopic, String connectionMessage){
  String ip = "ws://" + _host + ":" + String(_port);
  this->ip = ip;
  this->_reconnectMessage = connectionMessage;
  this->_reconnectTopic = connectTopic;
  webSocketClient.connect(ip);
  this->onConnect();
  webSocketClient.onEvent(onEventsCallback);
  webSocketClient.onMessage([](WebsocketsMessage msg){
    Serial.println("Got Message: " + msg.data()); 
  });
}

void AdaptorWSClass::onConnect(){
  this->send(_reconnectTopic, _reconnectMessage);
}

void AdaptorWSClass::send(String topic, String message){
  JSONVar Message;
  Message["event"]=topic;
  Message["data"]=message;
  webSocketClient.send(JSON.stringify(Message));
}

void AdaptorWSClass::reconnect(){
  Serial.println("Se desconectó");
  webSocketClient.connect(this->ip);
  if(webSocketClient.available()){
    this->onConnect();
  }
}

void AdaptorWSClass::loop(){
  webSocketClient.poll();
  if(!webSocketClient.available()){
    webSocketClient.close();
    this->reconnect();
    delay(300);
  }
}

AdaptorWSClass::~AdaptorWSClass(){}
