//CODE 12

//CODE TO BE DEPLOYED IN YELLOW ESP32.
//ESP32 SENDER ESP32 RECEIVER
//MAC ADDRESS 58:BF:25:36:97:48

#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <OneWire.h>
#include <DallasTemperature.h>


// RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x30, 0xC6, 0xF7, 0x28, 0x8A, 0x9C};
uint8_t alternateAddress[] = {0xC4, 0xDD, 0x57, 0xEA, 0x70, 0xCC}; 

uint8_t i=1; //used as packet number when sent to NODE 6
uint8_t j=1; //used as packet number when sent to NODE 10
const int oneWireBus = 0;
esp_now_peer_info_t peerInfo;
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int id;          //Unique ID i.e 12
  int num;         //packet number
  int sensorinfo;  //sensor data +120 constant
} struct_message;

struct_message myData;

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
  myData.id = 12;
  myData.num = (i-1);
  myData.sensorinfo = 120;
  if (status==ESP_NOW_SEND_FAIL){
    esp_now_send(alternateAddress,(uint8_t *) &myData, sizeof(myData));
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
  
  WiFi.printDiag(Serial); // Uncomment to verify channel number before
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial); // Uncomment to verify channel change after
  

  
  //Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.encrypt = false;
  
  //Add peer
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
  
}

void loop() {
//    sensors.requestTemperatures(); 
//    float temperatureC = sensors.getTempCByIndex(0);
    myData.id = 12;
    myData.num = i++;
//    myData.sensorinfo = temperatureC;
    myData.sensorinfo = 120;
    Serial.print(myData.sensorinfo);
    Serial.println("ºC");
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
    delay(5259);
    j++;
}
