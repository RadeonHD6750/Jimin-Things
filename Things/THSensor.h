/*
  2024-01-09

  온습도 센서

*/

#include <DHT.h>

#define DHTPIN 3
#define DHTTYPE DHT11


class THSensor{
  private:
    DHT *dhtSensor;
  public:
    THSensor();
    float readHumidity();
    float readTemperature();
};