//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <DHT.h>
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht (DHTPIN,DHTTYPE);

#define FIREBASE_HOST "imra-liv-142d5-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Rr4WC4vGNTdohLZ3va0AqpEAIVRiP3O6oaW71GnB"

// #define FIREBASE_HOST "imra-liv-default-rtdb.asia-southeast1.firebasedatabase.app"
// #define FIREBASE_AUTH "Vkd7Ivnzh669znqRPY0po9hjW15Q6IRQKyqQ3KdS"

#define WIFI_SSID "mathews"
#define WIFI_PASSWORD "12345678"
const int trigPin = D5; 
const int gaspin = A0; 
int lightpin = D7;


int light;
int gas;

int airq;
int ht;
int tm;

char readh[17];
char readt[17];
char readg[17];
char reade[17];

char readm[5];

String mega;

String mega_emergency;

bool e = 0;

long duration;
int distance;
String current_emergency;
void setup() {
  pinMode(DHTPIN,INPUT);
  dht.begin();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(gaspin, INPUT);
  pinMode(lightpin, INPUT);
  pinMode(D4,OUTPUT);
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // Serial.println("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    // Serial.print(".");
    delay(500);
  }
  // Serial.println();
  // Serial.print("connected: ");
  // Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("emergencies", "");
  Firebase.setInt("AIR_QUALITY", 0);
  Firebase.setInt("HUMIDITY", 0);
  Firebase.setInt("TEMPERATURE", 0);
  Firebase.setBool("EMERGENCY2", 0);
  // Serial.println("updated the firebase vars");
  
}

int ledDb;
char air;
char serial;
void loop() {
  serial = Serial.read();
  if(serial)
  
  ledDb=Firebase.getInt("led");
  if(ledDb==1){
    digitalWrite(D4,HIGH);
  }else{
    digitalWrite(D4,LOW);
  }
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  light = analogRead(D7);
  gas = analogRead(A0);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);


  // Serial.readBytesUntil('_', readg, 10);
  // Serial.readBytesUntil('.', readh, 10);
  // Serial.readBytesUntil('#', readt, 10);
  // Serial.readBytesUntil('!', reade, 10);

  Serial.readBytesUntil('.', readm, 5);


  // Serial.println(readg);
  // Serial.println(readh);
  // Serial.println(readt);
  // Serial.println(readt);

  // Serial.println("hellow");

  // Reads the echoPin, returns the sound wave travel time in microseconds


  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  // Serial.print(light);
  // Serial.print("Distance: ");




  // delay(2000);
  // delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // String airq_s = String(readg);
  // String ht_s = String(readh);
  String tm_s = String(readt);
  // String e_s = String(reade);
  mega = String(readm);

  mega_emergency = mega.substring(mega.length() - 1, mega.length());

  // airq = airq_s.substring(airq_s.length() - 3, airq_s.length()).toInt();

  // int em = airq_s.substring(airq_s.length() - 1, airq_s.length()).toInt();
  // if(em == 1){
  //   e = true;
  // }

  // if(em == 0){
  //   e = false;
  // }


  // // airq = airq_s.toInt();
  // ht = ht_s.toInt();
  // tm = tm_s.toInt();




  // Serial.println(airq_s);
  // Serial.println(ht_s);
  // Serial.println(tm_s);
  // Serial.println(airq);
  // Serial.println(ht);
  // Serial.println(tm);
  // Serial.print("mega:");
  // Serial.println(mega_emergency);
  // Serial.print("-");
  // Serial.print(mega_emergency);
  // Serial.println("-");



  // if (ht > 0){
  //   Firebase.setInt("HUMIDITY",ht);
  // }

  // if (tm > 0){
  //   Firebase.setInt("TEMPERATURE", tm);
  // }

  // if (airq > 0){
  //   Firebase.setInt("AIR_QUALITY", airq);
  // }
  // Firebase.setBool("EMERGENCY2", e);

  // Serial.println(e);

  

  if(mega_emergency == "C"){
      current_emergency = "High CO2";
  }
  else if(mega_emergency == "N" ){
      current_emergency = "None";
  }
  else if(mega_emergency == "H"){
      current_emergency = "High Humidity";
  }
  else if(mega_emergency == "T"){
      current_emergency = "High Temperature";
  }
  else if(mega_emergency == "Q"){
      current_emergency = "High CO2 And Humidity";
  }
  else if(mega_emergency == "W"){
      current_emergency = "High CO2 And Temperature";
  }
  else if(mega_emergency == "R"){
      current_emergency = "High Humidity And Temperature";
  }
  else if(mega_emergency == "E"){
      current_emergency = "High Humidity, Temperature And CO2";
  }
  // Serial.println(current_emergency);
  Firebase.setString("emergencies", current_emergency);
  // Firebase.setFloat("TEMPERATURE", tm);
  // Firebase.setFloat("HUMIDITY", h)
  String intruder = Firebase.getString("/Intruder Detection/Status");
  String emergency = Firebase.getString("/Emergency/emergency");
  int hr = Firebase.getInt("/fitness_data/heartRate");
  String back = Firebase.getString("/Controls/Back");
  String front = Firebase.getString("/Controls/Front");
  String left = Firebase.getString("/Controls/Left");
  String right = Firebase.getString("/Controls/Right");

  // Serial.print(intruder);
  // Serial.println(": intruder");
  // Serial.print(emergency);
  // Serial.println(": emergency");
  // Serial.print(hr);
  // Serial.println(": heartrate");
  // Serial.print(back);
  // Serial.println(": back");
  // Serial.print(front);
  // Serial.println(": front");
  // Serial.print(left);
  // Serial.println(": left");
  // Serial.print(right);
  // Serial.println(": right");

  String Direction;

  if(back == "True"){
    Direction = "B";
  }
  else if(front == "True"){
    Direction = "F";
  }
  else if(left == "True"){
    Direction = "L";
  
  }
  else if(right == "True"){
    Direction = "R";
  }
  else{
    Direction = "O";
  }


  Serial.print(intruder);
  Serial.print("*");
  Serial.print(emergency);
  Serial.print("#");
  Serial.print(Direction);
  Serial.println("!");

  readg[10] = {0};
  readh[10]= {0};
  readt[10]= {0};
  readm[5] = {0};
  memset(&(readg[0]), 0, 14);
  memset(&(readh[0]), 0, 14);
  memset(&(readt[0]), 0, 14);
  memset(&(readm[0]), 0, 5);


}
