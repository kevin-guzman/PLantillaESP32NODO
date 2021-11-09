#include <Arduino.h>

String byteToString(byte* payload, unsigned int length){
  String value;
  for (int i = 0; i < length; i++) {
    value+=((char)payload[i]);
  }
  return value;
}