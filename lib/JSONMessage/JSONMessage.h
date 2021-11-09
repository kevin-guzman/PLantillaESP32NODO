#ifndef jsonmessage_h
#define jsonmessage_h
#include <Arduino_JSON.h>

class JSONMessage{
  public:
    JSONMessage();
    virtual ~JSONMessage();
    void setMetric(String type, double value, String unity);
    void init(JSONVar node, const String token);
    String get();
    String getNode();
  private:
    JSONVar _message;
    JSONVar _metrics;
    JSONVar _node;
    String removeSemiColons(String str);
};


#endif