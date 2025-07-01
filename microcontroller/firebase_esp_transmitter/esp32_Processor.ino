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
  
  connectToWiFi();
 
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
  update_serial();
}
unsigned long previousMillis = 0;
unsigned long intervalMillis = 60;

void loop(){
 unsigned long currentMillis = millis(); 
 




 if (currentMillis - previousMillis >= intervalMillis) {
   Serial.println(intervalMillis);
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

   previousMillis = currentMillis;
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



void update_serial() {
    bool flow_controller = false;
    String inputString = "";  
    unsigned long startTime = millis();  

    Serial.println("Enter the new send TIME interval: ");
    while (!flow_controller) {
        if (Serial.available()) {
            char incomingByte = Serial.read();
            // receive a string via serial
            //'\n' check if there was a line break
            if (incomingByte == '\n') { 
                int receivedValue = inputString.toInt();
                Serial.print("Received Value: ");
                Serial.println(receivedValue);
                intervalMillis = receivedValue;
                flow_controller = true;
            } else {
                inputString += incomingByte;
            }

            
            startTime = millis();
        }

        
        if (millis() - startTime >= 3000) {
            Serial.println("No input received, using default value: 60");
            intervalMillis = 60;  
            flow_controller = true; 
        }
    }
}


void connectToWiFi() {
    Serial.println("Escolha uma opção:");
    Serial.println("1 - Scannear Wi-Fi da Proximidade");
    Serial.println("2 - Conectar em Wi-Fi Salvos");

    unsigned long startTime = millis(); 

    while (true) {
        if (Serial.available()) {
            int option = Serial.parseInt();
            Serial.read(); 

            if (option == 1) {
                scanWiFiNetworks();
                break; 
            } else if (option == 2) {
                connectToSavedWiFi();
                break; 
            } else {
                Serial.println("Opção inválida! Tente novamente.");
            }
        }

        
        if (millis() - startTime >= 3000) {
            Serial.println("Nenhuma escolha feita em 3 segundos. Conectando à primeira rede salva...");
            String ssid = wifi_ssid[0];
            String password = wifi_password[0];
            WiFi.begin(ssid.c_str(), password.c_str());

            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                Serial.print(".");
            }

            Serial.println();
            Serial.println("Conectado com sucesso!");
            break; 
        }
    }
}


void scanWiFiNetworks() {
    Serial.println("Scaneando redes Wi-Fi...");
    int numNetworks = WiFi.scanNetworks();
    Serial.println("Redes encontradas:");

    for (int i = 0; i < numNetworks; ++i) {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (Sinal: ");
        Serial.print(WiFi.RSSI(i));
        Serial.println(" dBm)");
    }

    Serial.println("Escolha uma rede para conectar (1 a " + String(numNetworks) + "):");
    while (true) {
        if (Serial.available()) {
            int networkChoice = Serial.parseInt();
            Serial.read(); 

            if (networkChoice >= 1 && networkChoice <= numNetworks) {
                connectToNetwork(WiFi.SSID(networkChoice - 1));
                break;
            } else {
                Serial.println("Escolha inválida! Tente novamente.");
            }
        }
    }
}

void connectToNetwork(String ssid) {
    Serial.print("Conectando à rede: ");
    Serial.println(ssid);

    int networkIndex = -1;
    int numNetworks = WiFi.scanNetworks();

    
    for (int i = 0; i < numNetworks; ++i) {
        if (WiFi.SSID(i) == ssid) {
            networkIndex = i;
            break;
        }
    }

    // Verifica se a rede é protegida
    if (networkIndex != -1 && WiFi.encryptionType(networkIndex) != WIFI_AUTH_OPEN) {
        Serial.println("Esta rede requer senha. Digite a senha:");
        String password = "";
        while (true) {
            if (Serial.available()) {
                char c = Serial.read();
                if (c == '\n') {
                    break; 
                } else {
                    password += c; 
                }
            }
        }

        WiFi.begin(ssid.c_str(), password.c_str());
    } else {
        WiFi.begin(ssid.c_str());
    }

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Conectado com sucesso!");
}

void connectToSavedWiFi() {
    Serial.println("Redes Wi-Fi salvas:");

    for (int i = 0; i < sizeof(wifi_ssid) / sizeof(wifi_ssid[0]); i++) {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(wifi_ssid[i]);
    }

    Serial.println("Escolha uma rede para conectar (1 a " + String(sizeof(wifi_ssid) / sizeof(wifi_ssid[0])) + "):");
    while (true) {
        if (Serial.available()) {
            int savedNetworkChoice = Serial.parseInt();
            Serial.read();

            if (savedNetworkChoice >= 1 && savedNetworkChoice <= sizeof(wifi_ssid) / sizeof(wifi_ssid[0])) {
                
                String ssid = wifi_ssid[savedNetworkChoice - 1];
                String password = wifi_password[savedNetworkChoice - 1];
                WiFi.begin(ssid.c_str(), password.c_str());

                while (WiFi.status() != WL_CONNECTED) {
                    delay(500);
                    Serial.print(".");
                }

                Serial.println();
                Serial.println("Conectado com sucesso!");
                break;
            } else {
                Serial.println("Escolha inválida! Tente novamente.");
            }
        }
    }
}


