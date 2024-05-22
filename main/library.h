const char* ssid = "iPhone";
const char* password = "12387645";

// Your restdb.io API endpoint and API key
const char* endpoint = "https://homem-7367.restdb.io/rest/data/664d5ce74c393347000010b2";
const char* apiKey = "664d5cb8f13d101478c37ba0";
const int blue =19;
const int green =21;
const int red =18;
// const int outputPin = 2;
//const int buttonPin = 4;
//int buttonState = 0; // Define your button pin here

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}


bool getDataFromDatabase() {
  Serial.println("get data from api");
  HTTPClient http;
  http.begin(endpoint);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-apikey", apiKey);
  
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    if (httpResponseCode == HTTP_CODE_OK) {
      String payload = http.getString();
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);
      if (!error) {
        const char* pcstate = doc["pcstate"];
        if (strcmp(pcstate, "ON") == 0) {
          Serial.println("pcstate=on");
          return true;
        } else if (strcmp(pcstate, "OFF") == 0) {
          Serial.println("pcstate=off");
          return false;
        }
      } else {
        Serial.println("Failed to parse JSON");
      }
    } else {
      Serial.print("HTTP GET request failed, error: ");
      Serial.println(httpResponseCode);
    }
  } else {
    Serial.println("Failed to connect to server");
  }
  http.end();
  return false;
}
bool postDataToDatabase(float humidity, float temperature) {
  Serial.println("Posting data to API");
  HTTPClient http;
  http.begin(endpoint);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-apikey", apiKey);
  
  DynamicJsonDocument doc(1024);
  doc["humidity"] = humidity;
  doc["temperature"] = temperature;
  
  String jsonData;
  serializeJson(doc, jsonData);
  
  int httpResponseCode = http.PUT(jsonData); // Change POST to PUT if you need to update
  if (httpResponseCode > 0) {
    if (httpResponseCode == HTTP_CODE_OK || httpResponseCode == HTTP_CODE_CREATED) {
      Serial.println("Data posted successfully.");
      http.end();
      return true;
    } else {
      Serial.print("HTTP POST request failed, error: ");
      Serial.println(httpResponseCode);
    }
  } else {
    Serial.println("Failed to connect to server");
  }
  http.end();
  return false;
}


void changeColor(int redLedState, int greenLedState, int blueLedState){
  digitalWrite(red, redLedState);
  digitalWrite(green, greenLedState);
  digitalWrite(blue, blueLedState);
}
void updatePCStateInDatabase(const char* state) {
  Serial.println("Updating PC state in the database");
  HTTPClient http;
  http.begin(endpoint);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-apikey", apiKey);
  
  DynamicJsonDocument doc(1024);
  doc["pcstate"] = state;
  
  String jsonData;
  serializeJson(doc, jsonData);
  
  int httpResponseCode = http.PUT(jsonData); // Change PUT to POST if required
  if (httpResponseCode > 0) {
    if (httpResponseCode == HTTP_CODE_OK || httpResponseCode == HTTP_CODE_CREATED) {
      Serial.println("PC state updated successfully.");
      http.end();
    } else {
      Serial.print("HTTP PUT request failed, error: ");
      Serial.println(httpResponseCode);
    }
  } else {
    Serial.println("Failed to connect to server");
  }
}
