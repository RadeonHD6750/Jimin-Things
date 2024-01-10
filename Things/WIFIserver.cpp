#include "WIFIserver.h"

ESP8266WebServer webServer(80);

const char* ap_ssid = "NEXTIT-Solution";
const char* ap_password = "12345678";
const int WIFI_MAX_SIZE = 10;

String wifiList[WIFI_MAX_SIZE];
String rssiList[WIFI_MAX_SIZE];

HTMLPage html;
String connectedSSID = "-1";
String connectedPassword = "-1";

String getConnectedSSID()
{
  return connectedSSID;
}

String getConnectedPasswordD()
{
  return connectedPassword;
}


ESP8266WebServer* getESP8266WebServer()
{
  return &webServer;
}

void startServer()
{
  Serial.println("Start AP");
  WiFi.softAP(ap_ssid, ap_password);
  IPAddress myIP = WiFi.softAPIP();
      
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  Serial.println("wifi scanning");
  webServer.on("/", indexController);
  webServer.on("/action_page", wifiListController); //action_page에 접속하면 handleForm 함수를 실행한다.
      
  Serial.println("start server");
  webServer.begin();
  Serial.println("HTTP server started");
}

//WIFI 목록
void scanWiFiList()
{
  int numberOfNetworks = WiFi.scanNetworks();
    
  for(int i =0; i<numberOfNetworks; i++){
    rssiList[i] = WiFi.RSSI(i);

    wifiList[i] = WiFi.SSID(i);
  }
}

//GetMapping("/")
void indexController()
{
  webServer.send(200, "text/html", prepareSelectWifiListPage());
}

//GetMapping("/action_page")
void wifiListController() 
{
  connectedSSID = webServer.arg("apName"); 
  connectedPassword = webServer.arg("apPw"); 

  Serial.print("apName:");
  Serial.println(connectedSSID);

  Serial.print("apPw:");
  Serial.println(connectedPassword);
    
  //여기서 EEPROM에 저장하기

  //EEPROM.put(connectedSSIDAddress, apName);
  //EEPROM.put(connectedPasswordAddress, apPw);
  webServer.send(200, "text/html", html.getReturn); //Send web page
}

//와이파이 스캔
String prepareSelectWifiListPage(){

  //여기서 EEPROM에 저장된 SSID, Password가 있다면 불러오기
  //불러와서 WEB에다 보여주기
  scanWiFiList();
  //와이파이 스캔 결과 만들기
  String strList ="<ul>";
  for(int i =0; i< WIFI_MAX_SIZE; i++){
      if(wifiList[i] != "")
      {
        strList += "<li><a name='" + wifiList[i] + "' onclick='select(this.name)'>" + wifiList[i] + " : "+ rssiList[i] + "</a> </li>";
      }
  }
  strList += "</ul>";

  String htmlPage = html.getWIFIList(strList);
      
  return htmlPage;
}



