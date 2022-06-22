//CODE 5

//CODE TO BE DEPLOYED IN YELLOW ESP32.
//ESP32 SENDER ESP32 RECEIVER
//MAC ADDRESS 08:3a:f2:a9:73:b8

#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

uint8_t broadcastAddress[] = {0x30, 0xC6, 0xF7, 0x28, 0x8A, 0x9C};  //EC:94:CB:6F:74:6C
//RECEIVER IS BLACK ESP32
//uint8_t received

typedef struct struct_message {
    int id; // must be unique for each sender board
    int num;   // serial number allows us to track the packet
    int sensorinfo;   //sensor data
    //time
} struct_message;

struct_message myData1;
struct_message myData2;
struct_message board9;

struct_message boardsStruct[1] = {board9};

esp_now_peer_info_t peerInfo;

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
}

int j=1;

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
  esp_now_register_recv_cb(OnDataRecv);
}
int i=1;
void loop() {
  int board4X = boardsStruct[0].num;
  int board4Y = boardsStruct[0].sensorinfo;
  // Set values to send
  myData1.id = 5;
  myData1.num = i++;
  myData1.sensorinfo = 50;
  Serial.println(myData1.sensorinfo);
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData1, sizeof(myData1));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(5650);
}
