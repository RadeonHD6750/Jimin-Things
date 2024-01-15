/*
2023-11-09 AP모드에서 향후 연결할 WIFI를 찾고 연결하기
2024-01-08 WIFI 부분 분리
2024-01-08 MQTT 연동

작성자 : 서지민

*/

//WEBServer
#include "WIFIserver.h"

//MQTT
#include "MQTTManager.h"

//Sensor
#include "THSensor.h"

String rootTopic = "/IoT/Sensor/jimin";
String sensorID = "jimin";

//WEB
ESP8266WebServer *server;


//Operating

int operatingMode = 1; // 1 - AP, 2 - STA
int operatingPin = D2;
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;  


//MQTT
MQTTManager mqtt;



//Sensor
THSensor th;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const long sensinginterval = 2000;

//AnalogRead
int analogPin = A0;

void networking();
void sensing();
bool operatingSwitchingDebounce();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Start Jimin IoT Things");

  pinMode(operatingPin, INPUT);
  pinMode(analogPin, INPUT);

  server = getESP8266WebServer();
  startServer();
}

void loop() { 
  currentMillis = millis();
  
  int reading = digitalRead(operatingPin);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = currentMillis;
  }

  if ((currentMillis - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      if(operatingMode == 2 && buttonState)
      {
        operatingMode = 1;
        Serial.println("Switching STA >>>> AP");
      }
      else if(operatingMode == 1 && buttonState)
      {
        operatingMode = 2;
        Serial.println("Switching AP >>>> STA");
      }
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;

  switch(operatingMode)
  {
    case 1:
      networking();
      break;


    case 2:
      sensing();
      break;
  }

}

void networking()
{
  server->handleClient();
}


void sensing()
{
  if (currentMillis - previousMillis >= sensinginterval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    String ssid = getConnectedSSID();

    if(ssid == "-1")
    {
      operatingMode = 1;
      return;
    }

    if(mqtt.isConnect())
    {
      int readAnalog = analogRead(analogPin);
      float temp = th.readTemperature();
      float hum = th.readHumidity();

      String readAnalogStr = String(readAnalog);
      String tempStr = String(temp);
      String humStr = String(hum);

      //publish
      String data = String("{\"sensor_id\" : \"" + sensorID +
       "\", \"analog\": \""+  readAnalogStr  + "\", \"temp\": \"" +  tempStr  + "\"}" );

      mqtt.publish(rootTopic, data);
      Serial.println(data);
    }
    else
    {
      mqtt.setupWIFI();
      delay(500);

      mqtt.connect();
    }

  }
}