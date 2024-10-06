#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>  // Include DHT library for temperature and humidity sensor

#define DHT_PIN 4     // DHT22 sensor data pin
#define DHTTYPE DHT22 // DHT22 sensor type
#define SOIL_PIN 34   // Analog pin for soil moisture sensor

// WiFi credentials
const char* ssid = "Redmi Note 12 pro";
const char* password = "12345678";

// Server URL to send data
String serverName = "http://192.168.125.127:5000/submit_data";

// DHT object for temperature and humidity
DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Initialize Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

// Function to calculate and invert soil moisture to percentage
float getSoilMoisturePercentage() {
  int analogValue = analogRead(SOIL_PIN);  // Read raw analog value
  float soilMoisturePercent = 100 - ((analogValue / 4095.0) * 100);  // Invert percentage
  return soilMoisturePercent;
}

void sendData(float temperature, float humidity, float soilMoisturePercent) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Prepare JSON data
    String postData = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + ", \"soil_moisture\": " + String(soilMoisturePercent) + "}";
    
    int httpResponseCode = http.POST(postData);
    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully.");
    } else {
      Serial.println("Error sending data.");
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

void loop() {
  // Read temperature and humidity from DHT22 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read soil moisture percentage
  float soilMoisturePercent = getSoilMoisturePercentage();

  // Print values to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisturePercent);
  Serial.println(" %");

  // Send data to the server
  sendData(temperature, humidity, soilMoisturePercent);

  // Delay before taking next readings
  delay(5000);  // 5-second delay
}
