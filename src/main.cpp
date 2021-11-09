#include <WiFi.h>
#include <JSONMessage.h>

#include <AdaptorMQTT.h>
#include <AdaptorWS.h>

#include "util/MetricTypes.h"
#include "util/UnityTypes.h"
#include "util/ParseFunctions.h"

//Sensores PinOut
#define lm41 3

// Editables
int WS_PORT = 8080;
String WS_SERVER = "192.168.0.13";
const String token = "_test_token";
String regionName = "Molinos";
int nodeId = 10;
String deviceId = "1120331231"; //cédula
const char* SSID = "FAMILIA GUSMAN";
const char* password = "10218715F";
const int baudRate = 115200;
double latitude = 4.5948510, longitude = -74.1585610;

// AdaptorMQTT MQTT(MQTT_SERVER, MQTT_PORT, clientId);
JSONMessage JSONmessage;
AdaptorWSClass wsAdaptor(WS_SERVER, WS_PORT);

MetricTypes MT; 
UnityTypes UT;

void setup(){
  Serial.begin(baudRate);
  delay(2000);
  Serial.println("");
  Serial.println("Proyect has started");
  Serial.println("Connecting to WiFi");
  WiFi.begin(SSID, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(400);
    Serial.print(".");
  }

  JSONVar node;
  node["nodeId"] = nodeId;
  node["deviceId"] = deviceId;
  node["regionName"] = regionName;
  node["mac"] = WiFi.macAddress();
  node["latitude"] = latitude;
  node["longitude"] = longitude;

  JSONmessage.init(node, token);

  wsAdaptor.init(WS_SERVER, WS_PORT, "node/connect", JSONmessage.getNode());
  delay(200);
}

float i =0.0;

void loop(){

  wsAdaptor.loop();
  // JSONmessage.setMetric(MT.temperatura, 20.5, UT.tepertatura.farenheit);
  JSONmessage.setMetric(MT.temperatura, i, UT.tepertatura.centigrados);

  JSONmessage.setMetric(MT.humedad, i*1.5, "9" + UT.humedad.porciento);
  
  JSONmessage.setMetric(MT.velocidad, i*0.2, UT.velocidad.ms);
  // Serial.println(JSONmessage.get());
  delay(10000);
  wsAdaptor.send("report/metrics", JSONmessage.get());
  if (i>=100.0){
    i=0.0;
  }else{
    i+=0.5;
  }

}