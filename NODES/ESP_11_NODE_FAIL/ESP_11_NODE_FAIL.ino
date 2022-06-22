//CODE 11

//CODE TO BE DEPLOYED IN THIRD ESP32 WITH TEMP.
//ESP32 SENDER ESP32 RECEIVER
//MAC ADDRESS 9c:9c:1f:c5:5f:34

#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

uint8_t broadcastAddress[] = {0x9C, 0x9C, 0x1F, 0xC5, 0x5F, 0x34};  //EC:94:CB:6F:74:6C
uint8_t alternateAddress[] = {0x0C, 0xB8, 0x15, 0xC4, 0x66, 0x8C}; 

uint8_t i=1; //used as packet number when sent to NODE 3
uint8_t j=1; //used as packet number when sent to NODE 4

typedef struct struct_message {
    int id; // must be unique for each sender board
    int num;   // serial number allows us to track the packet
    int sensorinfo;   //sensor data
    //time
} struct_message;

struct_message myData1;
struct_message myData2;
struct_message board8;

struct_message boardsStruct[1] = {board8};

esp_now_peer_info_t peerInfo;
// GPIO where the DS18B20 is connected to
const int oneWireBus =0;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

constexpr char WIFI_SSID[] = "Esp_wifi";

int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
      for (uint8_t i=0; i<n; i++) {
          if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
              return WiFi.channel(i);
          }
      }
  }
  return 0;
}


//Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  myData1.id = 11;
  myData1.num = (i-1);
  myData1.sensorinfo = 110;
  if (status==ESP_NOW_SEND_FAIL){
    esp_now_send(alternateAddress,(uint8_t *) &myData1, sizeof(myData1));
  }
}



void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData2, incomingData, sizeof(myData2));
  Serial.printf("Board ID %u: %u bytes\n", myData2.id, len);
  // Update the structures with the new incoming data
  boardsStruct[0].num = myData2.num;
  boardsStruct[0].sensorinfo = myData2.sensorinfo;
  boardsStruct[0].id= myData2.id;
  Serial.printf("num : %d \n", boardsStruct[0].num);
  Serial.printf("Sensor info : %d \n", boardsStruct[0].sensorinfo);
  Serial.println();

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &boardsStruct[0], sizeof(boardsStruct[0]));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}



void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  sensors.begin();

  //ESP_NOW initializing
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  int32_t channel = getWiFiChannel(WIFI_SSID);
  // Once ESPNow is successfully Init, we will register for Send CB to get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  //esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  
  WiFi.printDiag(Serial); // Uncomment to verify channel number before
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial); // Uncomment to verify channel change after
 

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
    memcpy(peerInfo.peer_addr, alternateAddress, 6);
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}


void loop() {
  int board8X = boardsStruct[0].num;
  int board8Y = boardsStruct[0].sensorinfo;
  // Set values to send
//  sensors.requestTemperatures(); 
//  float temperatureC = sensors.getTempCByIndex(0);
  myData1.id = 11;
  myData1.num = i++;
//  myData1.sensorinfo = temperatureC;
  myData1.sensorinfo = 110;
  Serial.print(myData1.sensorinfo);
  Serial.println("ºC");
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData1, sizeof(myData1));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(4496);
  j++;
}
