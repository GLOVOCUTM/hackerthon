#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include <DHT.h>
#include "library.h"

#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Servo servo;




void setup() {
 
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  
  Serial.begin(115200);
  servo.attach(2);
  dht.begin();
  connectToWiFi();
  changeColor(0,0,255);
}

void loop() {
  float humi = dht.readHumidity();
  float tempC = dht.readTemperature();
  
  if (isnan(humi) || isnan(tempC)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");
    Serial.print("  |  ");
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
    
    
    if (humi > 60) {
      
      servo.write(90);
      updatePCStateInDatabase("OFF");
      delay(2000);
    }
    
    
    if (WiFi.status() == WL_CONNECTED) {
      if (postDataToDatabase(humi, tempC)) {
        Serial.println("Data posted successfully.");
        delay(2000);
      } else {
        Serial.println("Failed to post data.");
      }
      delay(2000);

      if (getDataFromDatabase()) {
        Serial.println("The led is on");
        servo.write(45);
        changeColor(0,255,0);
      } else {
        Serial.println("The pin is set to low");
        servo.write(90);
        changeColor(255,0,0);
      }
    } else {
      Serial.println("WiFi not connected.");
      connectToWiFi();
    }
  }
  delay(30000); // Debounce delay
}
