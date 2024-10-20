/*
This code is uploaded at the remote/transmitter nodemcu. 
*/

#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE THE MAC Address
//Ex:- 12:34:56:78:90:ab - This is the mac address of reciever/car nodemcu.
//12:34:56:78:90:ab ---> {0x12, 0x34, 0x56, 0x78, 0x90, 0xab}
uint8_t broadcastAddress[] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab};

const int turnPin = A0;

//--- Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
  int turn;
  int move;
} struct_message;

// Create a struct_message called DHTReadings to hold sensor readings
struct_message controllerReadings;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setReadings(){
  int joyXValue;
  joyXValue = analogRead(turnPin);
  if (joyXValue < 512) {
      controllerReadings.move = map(joyXValue, 512, 1023, 0, 200);
  } else {
      controllerReadings.move = map(joyXValue, 512, 1023, 0, 200);
  }
  if(Serial.available()>0){
    String data = Serial.readStringUntil('\n');
    controllerReadings.turn = data.toInt();
  }  
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}
 
void loop() {
    //Set controller readings
  setReadings();

  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &controllerReadings, sizeof(controllerReadings));
}