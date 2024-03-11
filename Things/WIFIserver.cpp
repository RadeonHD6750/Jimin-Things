#include "WIFIserver.h"

ESP8266WebServer webServer(80);

const char* ap_ssid = "NEXTIT-Solution";
const char* ap_password = "12345678";
const int WIFI_MAX_SIZE = 15;

String wifiList[WIFI_MAX_SIZE];
int rssiList[WIFI_MAX_SIZE];
int scanningCount = 0;

HTMLPage html;
String connectedSSID = "next_office";
String connectedPassword = "nextit8850";

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
    
  scanningCount = 0;
  for(int i =0; i < numberOfNetworks; i++){
  
    String tempSSID = WiFi.SSID(i);
    tempSSID.trim();
    int tempRSSI = WiFi.RSSI(i);

    if(tempSSID != "" && tempSSID != " ")
    {
      wifiList[scanningCount] = tempSSID;
      rssiList[scanningCount] = tempRSSI;
      scanningCount++;
    }
  }

  for(int i =0; i < scanningCount; i++)
  {
    
    for(int j = 0; j < scanningCount - 1; j++)
    {
      if(rssiList[j] < rssiList[j + 1])
      {
        int temp = rssiList[j];
        rssiList[j] = rssiList[j + 1];
        rssiList[j + 1] = temp;

        String temp2 = wifiList[j];
        wifiList[j] = wifiList[j + 1];
        wifiList[j + 1] = temp2;
      }
    }
  }

}

//GetMapping("/")
void indexController()
{
  
  //여기서 EEPROM에 저장된 SSID, Password가 있다면 불러오기
  //불러와서 WEB에다 보여주기
  scanWiFiList();
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

  //와이파이 스캔 결과 만들기
  String strList ="<ul>";
  for(int i =0; i< scanningCount; i++){
      if(wifiList[i] != "")
      {
        strList += "<li><a name='" + wifiList[i] + "' onclick='select(this.name)'>" 
        + wifiList[i] +  " (" + rssiList[i] + ") </a> </li>";
      }
  }
  strList += "</ul>";

  String htmlPage = html.getWIFIList(strList);
      
  return htmlPage;
}


String getEEPROM(int address)
{
  char temp[20];
  EEPROM.get(address, temp);
    
  String tempStr(temp);

  tempStr.trim();
  return tempStr;
}

String getSSIDEEPROM()
{
  return getEEPROM(connectedSSIDAddress);
}

String getPWEEPROM()
{
  return getEEPROM(connectedPasswordAddress);
}


