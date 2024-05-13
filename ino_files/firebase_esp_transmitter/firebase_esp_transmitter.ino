// https://www.youtube.com/watch?v=6cWhEmgBhbo&ab_channel=ElectronicsSimplified
// config tutorial video, if an error occour


#include <Arduino.h>
#include <WiFi.h>               
#include <Firebase_ESP_Client.h>
#include <DHTesp.h>               
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define DHTPIN 33
#define MQ3_PIN 13
#define FIRE_SENSOR 26


DHTesp dht;

#define WIFI_SSID "FIAP-DINO"
#define WIFI_PASSWORD "dino#2017"
#define API_KEY "AIzaSyBbZHqxDbojbNKaDfLs7uR9NR84MzQEMQw" //project config
                                                          //don't forget to set the anonymous login (authentication tab) 
#define DATABASE_URL "https://flux-m-cca2f-default-rtdb.firebaseio.com/" //copy past over the realtime data base
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;                      

void setup(){
  
  dht.setup(DHTPIN, DHTesp::DHT11);
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

void loop(){

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  
  //bool alchool_sensor_reading = digitalRead(MQ3_PIN);
  //int fire_sensor_reading = digitalRead(FIRE_SENSOR);
  //Serial.println(fire_sensor_reading);
  Serial.println(temperature);
  
  dht_sensor_fb(temperature, humidity);
  //alchool_sensor_fb(alchool_sensor_reading);

  //fire_sensor_fb(fire_sensor_reading);

}

void fire_sensor_fb(bool fire_sensor){
  if (Firebase.RTDB.setInt(&fbdo, "FIRE", fire_sensor)){
      Serial.print("Fire : ");
      Serial.println(fire_sensor);
    }
    else {
      Serial.println("Failed to Read from the Sensor");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}
void alchool_sensor_fb(bool alchool_sensor){
  if (Firebase.RTDB.setInt(&fbdo, "MQ3", alchool_sensor)){
      Serial.print("Alchool : ");
      Serial.println(alchool_sensor );
    }
    else {
      Serial.println("Failed to Read from the Sensor");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}

void dht_sensor_fb(float temperature, float humidity){
  if (Firebase.RTDB.setFloat(&fbdo, "DHT11/Temperature", temperature)){
      Serial.print("Temperature : ");
      Serial.println(temperature);
    }
    else {
      Serial.println("Failed to Read from the Sensor");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    
    
    if (Firebase.RTDB.setFloat(&fbdo, "DHT11/Humidity", humidity)){
      Serial.print("Humidity : ");
      Serial.print(humidity);
    }
    else {
      Serial.println("Failed to Read from the Sensor");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}
