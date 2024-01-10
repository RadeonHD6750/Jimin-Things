/*
  2024-01-09

  MQTT 연동부

*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class MQTTManager{
  private:

    const char* mqtt_server = "nextit.or.kr";
    const int mqtt_port = 21883;

    const int MSG_BUFFER_SIZE = 200;

    String connectedSSID = "";
    String connectedPassword = "";

    WiFiClient *wifiClient;
    PubSubClient *pubClient;

  public:

    MQTTManager();
    void setupWIFI();
    void setupMQTT();
    bool isConnect();
    void connect();
    void reconnect();
    void publish(String topicStr, String payloadStr);
};


void receivedMQTTCallback(char* topic, byte* payload, unsigned int length);