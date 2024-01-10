
#include "WIFIserver.h"
#include "MQTTManager.h"



MQTTManager::MQTTManager()
{
  wifiClient = new WiFiClient();
  pubClient = new PubSubClient(*wifiClient);

  this->setupMQTT();
}

void MQTTManager::setupWIFI() {

  delay(10);

  connectedSSID =  getConnectedSSID();
  connectedPassword =  getConnectedPasswordD();


  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(connectedSSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(connectedSSID, connectedPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void MQTTManager::setupMQTT()
{
  pubClient->setServer(mqtt_server, mqtt_port);
  pubClient->setCallback(receivedMQTTCallback);
}

void MQTTManager::reconnect() {
  // Loop until we're reconnected
  while (!pubClient->connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "NEXTIT-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (pubClient->connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      pubClient->publish("outTopic", "hello world");
      // ... and resubscribe
      pubClient->subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(pubClient->state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

bool MQTTManager::isConnect()
{
  return pubClient->connected();
}

void MQTTManager::connect()
{
  if (!isConnect()) 
  {
    reconnect();
  }
}

void MQTTManager::publish(String topicStr, String payloadStr)
{
  int topicLength = topicStr.length() + 1;
  int payloadLength = payloadStr.length() + 1;

  char *topic = new char[topicLength + 2];
  char *payload = new char[payloadLength + 2];
  
  topicStr.toCharArray(topic, topicLength);
  payloadStr.toCharArray(payload, payloadLength);

  pubClient->loop();
  pubClient->publish(topic, payload);

  String log = String("publish topic :" + topicStr + " msg : " + payloadStr);
  Serial.println(log);

  delete topic;
  delete payload;
}


void receivedMQTTCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}