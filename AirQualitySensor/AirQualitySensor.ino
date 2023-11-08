/*
  Jimin's AQM
  
  DTH11 - Temperature , Humidity
  TM1638 - 7LED
  GP2Y1010AU0F - Dust
*/

#include <dht11.h>
#include <TM1638.h>

#define DHT11PIN 2
#define DUST_CLEAN_VOLTAGE 0.65 // Dust Clean Voltage Init Value
#define AQM_Sampling_Time 2000

dht11 DHT11;
TM1638 TM1638module(9,10,11);

int measurePin = A0; //Connect dust sensor to Arduino A0 pin
int ledPower = 3;   //Connect 3 led driver pins of dust sensor to Arduino D3
  
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
  
float voMeasured = 0;
float calcVoltage = 0;

int Temperature = 0;
int Humidity = 0;
int Dust_Density = 0; //ug/m3

void DHT11_Function(); 
void TM1638_Function();
void GP2Y1010AU0F_Function();
float mapf(float x, float in_min, float in_max, float out_min, float out_max);

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
  //TM1638_Function();
  
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
  digitalWrite(ledPower,LOW); // power on the LED
   
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin); // read the dust value
  delayMicroseconds(deltaTime);
  
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  
  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = mapf(voMeasured,0,1023,0,5);
  
  
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  //dustDensity = 0.17 * calcVoltage - 0.1;
  Dust_Density = (calcVoltage - DUST_CLEAN_VOLTAGE) / 0.005;
  
  Serial.print("Dust Density: ");
  Serial.println(Dust_Density); // unit: ug/m3

}


void TM1638_Function()
{
  int display_Temperature = Temperature * 100000;
  int display_Humidity = Humidity * 1000;
  int display_Dust_Density = Dust_Density;
  
  int display_Number = display_Temperature + display_Humidity + display_Dust_Density;
  
  TM1638module.setDisplayToDecNumber(display_Number,0b00000000,false);
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

