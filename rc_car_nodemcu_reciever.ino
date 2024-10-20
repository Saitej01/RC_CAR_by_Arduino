/*
This code is uploaded at the reciever/car nodemcu.
*/

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Servo.h>

Servo steering;
int motor1ena = D5;
int motor2ena = D6;
int motor1pin1 = D0;
int motor1pin2 = D1;
int motor2pin1 = D2;
int motor2pin2 = D3;

//temporary variables
int move0;
int turn0;

// REPLACE THE MAC Address
//Ex:- 12:34:56:78:90:ab - This is the mac address of sender/transmitter/remote nodemcu.
//12:34:56:78:90:ab ---> {0x12, 0x34, 0x56, 0x78, 0x90, 0xab}
uint8_t broadcastAddress[] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab};

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
  int turn;
  int move;
} struct_message;

// Create a struct_message to hold incoming sensor readings
struct_message carReadings;

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&carReadings, incomingData, sizeof(carReadings));
  /*Serial.print("Bytes received: ");
  Serial.println(len);*/
  move0 = carReadings.move;
  turn0 = carReadings.turn;
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
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  //motor and servo connections intialization
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(motor1ena, OUTPUT); 
  pinMode(motor2ena, OUTPUT);
  steering.attach(D4);
}
 
void loop() {
  steering.write(90+turn0);
  if(move0>0){
    analogWrite(motor1ena, move0);
    analogWrite(motor2ena, move0);
    forward();
  }else if(move0<=0){
    analogWrite(motor1ena, -1*move0);
    analogWrite(motor2ena, -1*move0);
    backward();
  }
  delay(30);
}

void forward(){
  digitalWrite(motor1pin1, 1);
  digitalWrite(motor1pin2, 0);
  digitalWrite(motor2pin1, 1);
  digitalWrite(motor2pin2, 0);
}
void backward(){
  digitalWrite(motor1pin1, 0);
  digitalWrite(motor1pin2, 1);
  digitalWrite(motor2pin1, 0);
  digitalWrite(motor2pin2, 1);
}