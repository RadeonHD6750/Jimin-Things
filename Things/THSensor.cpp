#include "THSensor.h"

THSensor::THSensor()
{
  dhtSensor = new DHT(DHTPIN, DHTTYPE);

  dhtSensor->begin();

  Serial.println("Start DHT11");
}

float THSensor::readHumidity()
{
  return dhtSensor->readHumidity();
}
float THSensor::readTemperature()
{
  return dhtSensor->readTemperature();
}