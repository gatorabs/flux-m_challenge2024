// https://www.youtube.com/watch?v=6cWhEmgBhbo&ab_channel=ElectronicsSimplified
// config tutorial video, if an error occour
// Firebase library: https://github.com/rolan37/Firebase-ESP-Client-main
// DHTesp library: https://github.com/beegee-tokyo/DHTesp

#include <Arduino.h>
#include <WiFi.h>               
#include <Firebase_ESP_Client.h>
#include <DHTesp.h> 
#include  <Wire.h>              
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define MPU 0x68
#define DHTPIN 2
#define MQ3_PIN 26
#define MQ4_PIN 34
#define FIRE_SENSOR 15
#define LDR_PIN 32
DHTesp dht;

const char* wifi_ssid[] = {"FIAP-DINO", "Galaxyaa", "VIVOFIBRA-18B6"};
const char* wifi_password[] = {"dino#2017", "banana123", "3dd92218b6"};


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

int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup(){
  
  dht.setup(DHTPIN, DHTesp::DHT11);
  
  pinMode(FIRE_SENSOR, INPUT);
  pinMode(MQ3_PIN, OUTPUT);
  Serial.begin(115200);
  
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  
  Wire.write(0);    
  Wire.endTransmission(true);
  
  Serial.println("Selecione uma rede Wi-Fi:");
  for (int i = 0; i < 3; i++) {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(wifi_ssid[i]);
  }

  int escolha = 0;
  while (escolha < 1 || escolha > 3) {
    while (Serial.available() == 0) {
      
    }
    escolha = Serial.parseInt();
  }
  

  Serial.print("Conectando-se à rede: ");
  Serial.println(wifi_ssid[escolha - 1]);
  
  WiFi.begin(wifi_ssid[escolha - 1], wifi_password[escolha - 1]);

  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nConectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

 
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

 if (currentMillis - previousMillis >= 1) {
   previousMillis = currentMillis;
   float humidity = dht.getHumidity();
   float temperature = dht.getTemperature();
    
   int ldr_reading = analogRead(LDR_PIN);
   int ldr_mapping = map(ldr_reading, 0, 4095, 0, 100);
   int mq3_reading = digitalRead(MQ3_PIN);
    
   int mq4_reading = analogRead(MQ4_PIN);
   int mq4_mapping = map(mq4_reading, 2000, 4095, 0, 100);
    
   int fire_sensor_reading = digitalRead(FIRE_SENSOR);
   
   gyroscope_reading();
   int AcX_mapping = map(AcX, -12000,12000,0,180);
   int AcY_mapping = map(AcY, -12000,12000,0,180);
    
   firebase_sending_function("MQ4", mq4_mapping);
   firebase_sending_function("LDR", ldr_mapping);
   firebase_sending_function("LDR_pure", ldr_reading);

   firebase_sending_function("DHT11/Temperature", temperature);
   firebase_sending_function("DHT11/Humidity", humidity);
   firebase_sending_function("FIRE", fire_sensor_reading);
   
   firebase_sending_function("GYRO/x", AcY_mapping-90);
   firebase_sending_function("GYRO/y", AcX_mapping-90);
   //firebase_sending_function("GYRO/z", GyZ);
    
   firebase_sending_function("MQ3", MQ3_PIN);
 }
}

void gyroscope_reading(){
  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true); 
  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();
    
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  
  
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