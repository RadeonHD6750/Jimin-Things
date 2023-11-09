#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define DHTPIN 10     // 온습도 센서가 10번에 연결

/*
http://www.devicemart.co.kr/
#define DHTPIN 10     // 온습도 센서가 10번에 연결
#define DHTTYPE DHT22   // DHT22 온습도 센서 사용
*/

// 미세먼지 없을때 초기 V 값
// 공기청정기 위 등에서 먼지를 가라앉힌 후 voltage값 개별적으로 측정필요

#include "SimpleKalman.h"

SimpleKalman *dustKalman;
SimpleKalman *tempKalman;
SimpleKalman *fahKalman;
SimpleKalman *humKalman;


DHT dht(DHTPIN, DHTTYPE);
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
float no_dust = 0.28; //최초 전원 인가 후 깨끗한 환경에서의 지속적으로 측정되는 전압값 넣어주세요 

int dustout=A4; // 센서의 5번핀 먼지를 측정 아두이노 A4번에 연결

int v_led=A5; // 센서의 3번핀을 아두이노 A5번에 연결  센서 내부 적외선 led를 on/off 제어
int s_ledr=13; // 공기 상태에 나쁨 RED
int s_ledg=12; // 공기 상태에 보통 Green
int s_ledb=11; // 공기 상태에 좋음 Blue

float sensor_voltage = 0;
float filtered_dust = 0;
float filterd_hum = 0;
float filterd_temp = 0;
float filterd_fah = 0;

void setup()
{
 Serial.begin(9600); // 통신속도 9600bps로 시리얼 통신 시작

 pinMode(v_led,OUTPUT); // 적외선 led 출력으로 설정
 pinMode(s_ledr,OUTPUT); // LED Red 출력 설정
 pinMode(s_ledg,OUTPUT); // LED green 출력 설정
 pinMode(s_ledb,OUTPUT); // LED blue 출력 설정

  dht.begin();

 lcd.begin(16, 2);   
 lcd.setCursor(0,0);
 lcd.clear();
 digitalWrite(v_led,LOW); // 적외선 LED ON
 digitalWrite(s_ledr,LOW);
 digitalWrite(s_ledb,LOW);
 digitalWrite(s_ledg,LOW);

 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Initialize..."); 

 float dust_density_mean = 0;
 for(int i = 0; i < 100; i++)
 {
  float current_vo_value = get_sensor_analog();
  float sensor_voltage = get_voltage(current_vo_value); // get_voltage 함수 실행
  float dust_density = get_dust_density(sensor_voltage); // get_dust_density 함수 실행

  dust_density_mean = dust_density_mean + dust_density ;
 
  delay(10);
 }
 dust_density_mean = dust_density_mean / 100;

 dustKalman = new SimpleKalman(0, 50, 4, dust_density_mean);


 float h = dht.readHumidity(); //습도값을 h에 저장
 float t = dht.readTemperature(); //온도값을 t에 저장
 float f = dht.readTemperature(true);

  tempKalman = new SimpleKalman(0, 10, 4, t);
  humKalman = new SimpleKalman(0, 10, 4, h);
  fahKalman = new SimpleKalman(0, 10, 4, f);

 lcd.clear();
 lcd.print("clean "); 
 lcd.print(no_dust); 
 lcd.print("V"); 


 delay(3000);

}
void loop()
{
 /*
    데이터 시트에 나와있듯이 샘플링을 위해서 적외선 LED는 0.32ms 동안 펄스를 유지해야하며,
    0.28ms이후에 데이터를 읽을 수 있습니다.
 */

  //lcd.clear();

  //먼지
  float raw = get_sensor_analog();
  sensor_voltage = get_voltage(raw); // get_voltage 함수 실행
  if(sensor_voltage >= no_dust)
  {
    float dust_density = get_dust_density(sensor_voltage); // get_dust_density 함수 실행

    filtered_dust = dustKalman->FilterUpdate(dust_density);
  }

  //온습도
  float h = dht.readHumidity(); //습도값을 h에 저장
  float t = dht.readTemperature(); //온도값을 t에 저장
  float f = dht.readTemperature(true);
  if (!isnan(h) && !isnan(t) && h <= 100 && t <= 50) 
  {
    filterd_hum = humKalman->FilterUpdate(h);
    filterd_temp = tempKalman->FilterUpdate(t);
    filterd_fah = fahKalman->FilterUpdate(t);

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(filterd_fah, filterd_hum);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(filterd_temp, filterd_hum, false);
  }


  //출력
  lcd.setCursor(0, 1);
  lcd.print("D:"); 
  //lcd.setCursor(5, 1);
  lcd.print(filtered_dust);
  //lcd.setCursor(8, 1);
  lcd.print("ug ");
  //lcd.setCursor(1, 11);
  lcd.print(sensor_voltage);
  lcd.print("V");

   Serial.print(filtered_dust);
   Serial.println(" [ug/m^3]");
   Serial.print("Voltage = ");
   Serial.print(sensor_voltage);
   Serial.println(" [V]");
   Serial.print("Dust Density = ");
   Serial.print(filtered_dust);
   Serial.println(" [ug/m^3]");
   Serial.println();

   
   Serial.print("Temperature: ");
   Serial.print(filterd_temp); //온도값 출력
   Serial.println("C");
   lcd.setCursor(0, 0);
   lcd.print("T:");
   //lcd.setCursor(5, 0);
   lcd.print(filterd_temp);
   //lcd.setCursor(7, 0);
   
   Serial.print("Humidity: "); //문자열 출력
   Serial.print(filterd_hum); //습도값 출력
   Serial.println("%");
   //lcd.setCursor(9, 0);
   lcd.print(" H:");
   //lcd.setCursor(13, 0);
   lcd.print(filterd_hum);
   //lcd.setCursor(15, 0);
   
  
   if(filtered_dust < 15)
   {
    digitalWrite(s_ledr,LOW);
    digitalWrite(s_ledb,HIGH);
    digitalWrite(s_ledg,LOW);
    Serial.println("LED B");
   }
   else if(filtered_dust < 35){
    digitalWrite(s_ledr,LOW);
    digitalWrite(s_ledb,LOW);
    digitalWrite(s_ledg,HIGH) ;
    Serial.println("LED G");
   }
   else 
   {
    digitalWrite(s_ledr,HIGH);
    digitalWrite(s_ledb,LOW);
    digitalWrite(s_ledg,LOW);
    Serial.println("LED R");
   }

  delay(5000);
}

float get_sensor_analog()
{
  digitalWrite(v_led,LOW); 
  delayMicroseconds(280); // 280us동안 딜레이
  float vo_value=analogRead(dustout); // 데이터를 읽음
  delayMicroseconds(40); // 320us - 280us
  digitalWrite(v_led,HIGH); // 적외선 LED OFF
  delayMicroseconds(9680); // 10us(주기) -320us(펄스폭) 한 값

  return vo_value;
}


float get_voltage(float value)
{
 float V= value * 5.0 / 1024; // 아날로그값을 전압값으로 바꿈
 return V;
}

float get_dust_density(float voltage)
{
 float dust=(voltage-no_dust) / 0.005; // 데이터 시트에 있는 미세먼지 농도(ug) 공식 기준
 return dust;
}

float get_dust()
{
  
 float current_vo_value = get_sensor_analog();
 float sensor_voltage = get_voltage(current_vo_value); // get_voltage 함수 실행


 float dust_density = get_dust_density(sensor_voltage); // get_dust_density 함수 실행

 return  dust_density;
}
