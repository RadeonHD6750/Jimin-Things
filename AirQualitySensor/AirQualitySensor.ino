/*
  Jimin's AQM
  
  DTH11 - Temperature , Humidity
  TM1638 - 7LED
  GP2Y1010AU0F - Dust
*/

#include <dht11.h>
#include <TM1638.h>
#include "DustSensor.h"

#define AQM_Sampling_Time 2000

DustSensor dust;
dht11 DHT11;


int Temperature = 0;
int Humidity = 0;
int Dust_Density = 0; //ug/m3

void DHT11_Function(); 
void GP2Y1010AU0F_Function();

void setup()
{
  
  Serial.begin(9600);
  Serial.println("DHT11 TEST");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
  
}


void loop()
{
  
  DHT11_Function();
  GP2Y1010AU0F_Function();
  
  delay(AQM_Sampling_Time);
}



void DHT11_Function()
{
  Serial.println("\n");

  int chk = DHT11.read(DHT11PIN);

  Serial.print("Read sensor: ");
  switch (chk)
  {
    case 0: 
        Serial.println("OK"); 
        break;
        
    case -1: 
        Serial.println("Checksum error"); 
        break;
        
    case -2: 
        Serial.println("Time out error"); 
        break;
        
    default: 
        Serial.println("Unknown error"); 
        break;
  }

  //Temperature
  Serial.print("Temperature (oC): ");
  Serial.println(DHT11.temperature, 1);
  Temperature = DHT11.temperature;

  //Humdity
  Serial.print("Humidity (%): ");
  Serial.println(DHT11.humidity, 1);
  Humidity = DHT11.humidity;

}

void GP2Y1010AU0F_Function()
{
 
  float Dust_Density = dust.getMeasuring();
  
  Serial.print("Dust Density: ");
  Serial.println(Dust_Density); // unit: ug/m3

}