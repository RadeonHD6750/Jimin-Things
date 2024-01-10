/*
  2024-01-08

  HTML 소스부분 분리
*/

#ifndef HTML_H
#define HTML_H

#include <Arduino.h>

class HTMLPage{

  private:
    //항상 쓰이는 헤더 부분
    String header = String("<!DOCTYPE HTML>") +
                "<html> <head>" +
                "<meta http-equiv=\"Content-Type\" Content=\"text/html; charset=utf-8\">\r\n" +
                "<meta name='viewport' content='width=device-width, initial-scale=1, minimum-scale=1, maximum-scale=1, user-scalable=no'>\r\n" + 
                "<title>Jimin's IOT</title>\r\n"+
                "<style>\r\n"+
                ".a:hover{cursor:pointer;}\r\n"+
                "div {margin-top:3%;}\r\n"+
                "input {margin-top:3%; font-size: 1.2rem;}\r\n"+
                "li {margin-top:3%; font-size: 1.2rem;}\r\n"+
                "</style></head>\r\n";
  public:

    //body 부분
    String getReturn = header + 
    "<body><div style='text-align:center; margin-top:10% font-size:2rem;'><a href='/' style='font-size:2rem;'> 돌아가기 </a></div></body>";

    String getWIFIList(String strList)
    {
      String htmlPage = header +
                "<body>\r\n"+
                "<h1>스캔한 WIFI 목록</h1>\r\n"+
                "연결가능한 WiFi를 선택하세요. <br>\r\n"+
                "<div id='wifiList'>"+ strList + "</div>\r\n"
                "<div id='selected'></div>\r\n"
                "<div id='_pw'>\r\n"+
                "<form method='get' action='/action_page'>\r\n"+
                "<input type='text' id='ap' name='apName'> 을 선택하셨습니다. 아래에 비밀번호를 입력하세요. <br>\r\n"+
                "<input type='password' id='pw' name='apPw'> <input type='submit' value='connect'>\r\n"+
                "</form>\r\n"+
                "</div>\r\n"+
                "</body>\r\n"+
                "<script>\r\n"+
                "document.getElementById('_pw').style.visibility = 'hidden';\r\n"+
                "function select(apName){\r\n"+
                "document.getElementById('ap').value = apName;\r\n"+
                "document.getElementById('_pw').style.visibility = 'visible';\r\n"+
                "document.getElementById('pw').value = ''}\r\n"+
                "function connection(){ \r\n"+
                "var pw = document.getElementById('pw').value;\r\n"+
                "} \r\n"+ 
                "</script> \r\n"+
                "</html> \r\n";

      return htmlPage;
    };
};

#endif