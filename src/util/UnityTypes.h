#include <Arduino.h>
#include "MetricTypes.h"

#ifndef UnityTypes_h
#define UnityTypes_h

typedef struct{
  struct{
    String centigrados = "celsius";
    String farenheit = "farenheit";
  }tepertatura;
  struct{
    String ms = "m/s";
  }velocidad;
  struct{
    String porciento = "%";
  }humedad;
}UnityTypes;

#endif