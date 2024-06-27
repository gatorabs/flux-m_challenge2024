// https://www.youtube.com/watch?v=6cWhEmgBhbo&ab_channel=ElectronicsSimplified
// config tutorial video, if an error occour
// Firebase library: https://github.com/rolan37/Firebase-ESP-Client-main
// DHTesp library: https://github.com/beegee-tokyo/DHTesp

#include <Arduino.h>
#include <WiFi.h>               
#include <Firebase_ESP_Client.h>
#include <DHTesp.h>               
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define DHTPIN 33
#define MQ3_PIN 13
#define MQ4_PIN 32
#define FIRE_SENSOR 26
#define LASER_PIN 15
#define LDR_PIN 34
DHTesp dht;

#define WIFI_SSID "VIVOFIBRA-18B6"
#define WIFI_PASSWORD "3dd92218b6"
#define API_KEY "AIzaSyBbZHqxDbojbNKaDfLs7uR9NR84MzQEMQw" //project config
                                                          //don't forget to set the anonymous login (authentication tab) 
#define DATABASE_URL "https://flux-m-cca2f-default-rtdb.firebaseio.com/" //copy past over the realtime data base
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;                      
bool is_laserOn = true;
void setup(){
  
  dht.setup(DHTPIN, DHTesp::DHT11);
  pinMode(LASER_PIN, OUTPUT);
  pinMode(FIRE_SENSOR, INPUT);
  pinMode(MQ3_PIN, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

 
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}
unsigned long previousMillis = 0;

void loop(){
 unsigned long currentMillis = millis(); 

if (currentMillis - previousMillis >= 100) {
  previousMillis = currentMillis;
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  int ldr_reading = analogRead(LDR_PIN);
  int mq4_reading = analogRead(MQ4_PIN);
  int mq4_mapping = map(mq4_reading, 0, 1023, 0, 100);
  int fire_sensor_reading = digitalRead(FIRE_SENSOR);
  
  digitalWrite(LASER_PIN, HIGH);
  
  //bool alchool_sensor_reading = digitalRead(MQ3_PIN);
  
  //Serial.println(fire_sensor_reading);
  
  

  
  //dht_sensor_fb(temperature, humidity);
  firebase_sending_function("MQ4", mq4_mapping);
  firebase_sending_function("LDR", ldr_reading);
  firebase_sending_function("DHT11/Temperature", temperature);
  firebase_sending_function("DHT11/Humidity", humidity);
  firebase_sending_function("FIRE", humidity);

  
  
  }

}


void firebase_sending_function(String firebase_ref, int sensor_ref){
  if (Firebase.RTDB.setInt(&fbdo, firebase_ref, sensor_ref)){
      Serial.print(firebase_ref + ":");
      Serial.println(sensor_ref);
    }
    else {
      Serial.println("Failed to Read from the Sensor");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}
