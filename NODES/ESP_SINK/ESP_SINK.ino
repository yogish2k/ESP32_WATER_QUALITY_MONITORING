#include <esp_now.h>
#include <WiFi.h>
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>


typedef struct struct_message {
  int id;
  int num;
  int sensorinfo;
}struct_message;

struct_message myData;
struct_message board2;
struct_message board3;
struct_message board4;
struct_message board5;
struct_message board6;
struct_message board7;
struct_message board8;
struct_message board9;
struct_message board10;
struct_message board11;
struct_message board12;

// Create an array with all the structures
struct_message boardsStruct[11] = {board2, board3, board4, board5, board6, board7, board8, board9, board10, board11, board12};

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
  
WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

void connectAWS()
{
  WiFi.mode(WIFI_STA);

  Serial.println("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  
  Serial.println(" AWS IoT Connected!");
}
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
//  String msg = "";
//  msg+=macStr;
  memcpy(&myData, incomingData, sizeof(myData));
//  msg+=",Board ID:";
//  msg+= String(myData.id);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-2].num = myData.num;
  boardsStruct[myData.id-2].sensorinfo = myData.sensorinfo;
//  msg+=",Packet_No:";
//  msg+=String(boardsStruct[myData.id-2].num);
//  msg+=",SensorInfo:";
//  msg+=String(boardsStruct[myData.id-2].sensorinfo);
  
 
  Serial.println();
//  client.publish(AWS_IOT_PUBLISH_TOPIC, msg);
  StaticJsonDocument<200> doc;
  doc["Board ID"] = myData.id;
  doc["Packet ID"] = boardsStruct[myData.id-2].num;
  doc["Sensor Info"] = boardsStruct[myData.id-2].sensorinfo;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  Serial.println(jsonBuffer);

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}


void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);

  //ESP-NOW Initializing
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.println(WiFi.channel());
  connectAWS();
  esp_now_register_recv_cb(OnDataRecv);

}

void loop() {
  // put your main code here, to run repeatedly:
  int board2X = boardsStruct[0].num;
  int board2Y = boardsStruct[0].sensorinfo;
  int board3X = boardsStruct[1].num;
  int board3Y = boardsStruct[1].sensorinfo;
  int board4X = boardsStruct[2].num;
  int board4Y = boardsStruct[2].sensorinfo;
  int board5X = boardsStruct[3].num;
  int board5Y = boardsStruct[3].sensorinfo;
  int board6X = boardsStruct[4].num;
  int board6Y = boardsStruct[4].sensorinfo;
  int board7X = boardsStruct[5].num;
  int board7Y = boardsStruct[5].sensorinfo;
  int board8X = boardsStruct[6].num;
  int board8Y = boardsStruct[6].sensorinfo;
  int board9X = boardsStruct[7].num;
  int board9Y = boardsStruct[7].sensorinfo;
  int board10X = boardsStruct[8].num;
  int board10Y = boardsStruct[8].sensorinfo;
  int board11X = boardsStruct[9].num;
  int board11Y = boardsStruct[9].sensorinfo;
  int board12X = boardsStruct[10].num;
  int board12Y = boardsStruct[10].sensorinfo;
  
  client.loop();
  delay(1000);
}
