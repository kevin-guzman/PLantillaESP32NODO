#include <Arduino_JSON.h>
#include "JSONMessage.h"

JSONVar _message;
JSONVar _metrics;
JSONVar _node;

JSONMessage::JSONMessage(){}

JSONMessage::~JSONMessage(){}

String JSONMessage::removeSemiColons(String str){
  String removedSemicolonsChain;
  for (int i = 0; i < str.length(); i++){
    if(str[i] != '"'){
      removedSemicolonsChain+=str[i];
    }
  }
  return removedSemicolonsChain;
}

void JSONMessage::setMetric(String type, double value, String unity){
  const int length = _metrics.length(); 
  bool alreadyExist = false;
  int index = 0;
  if(length > -1){
    for(int i=0; i < length; i++){
      JSONVar element = _metrics[i];
      const String metricType = removeSemiColons(JSON.stringify(element["type"]));
      if(metricType == type){
        alreadyExist=true;
        index=i;
      }
    }
  }
  JSONVar metric;
  metric["type"] = type;
  metric["value"] = value;
  metric["unity"] = unity;
  const int indexInMetrics = alreadyExist?index: length == -1 ? 0 : length;
  _metrics[indexInMetrics] = metric;
};

void JSONMessage::init(JSONVar node, const String token){
  _node = node;
  int nodeId = node["nodeId"];
  _message["token"] = token;
  _message["nodeId"] = nodeId;
};

String JSONMessage::get(){
  _message["metrics"] = _metrics;
  return JSON.stringify(_message);
};

String JSONMessage::getNode(){
  return JSON.stringify(_node);
}