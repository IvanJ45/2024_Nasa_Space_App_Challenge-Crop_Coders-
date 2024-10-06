#include "ESP32Servo.h"
#include <WiFi.h>
#include <HTTPClient.h>

// Servo motor setup
Servo myservo;
int servoPin = 15;  // Pin connected to the servo

// Ultrasonic sensor setup
#define TRIG_PIN 14  // Ultrasonic TRIG pin
#define ECHO_PIN 12  // Ultrasonic ECHO pin

// Rain sensor setup
#define RAIN_PIN 34   // Analog pin for rain sensor

// WiFi credentials
const char* ssid = "Redmi Note 12 pro";
const char* password = "12345678";

// Server URL to send data
String serverName = "http://192.168.125.127:5000/submit_data";

// Function to calculate distance using the ultrasonic sensor
float getDistance() {
  long duration;
  float distanceCm;

  // Trigger the ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);   // Ensure it's LOW for a short time before pulsing
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);  // Pulse HIGH for at least 10 microseconds
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin and calculate distance
  duration = pulseIn(ECHO_PIN, HIGH, 30000L);  // Set a timeout to avoid infinite waiting
  distanceCm = (duration * 0.034) / 2;  // Convert time to distance (cm)

  // Handle invalid readings and limit the range to 2-30 cm
  if (duration == 0 || distanceCm < 2 || distanceCm > 30) {
    return 0;  // Return 0 for invalid distance
  } else {
    return distanceCm;  // Valid reading
  }
}

// Function to calculate rain intensity percentage
int getRainIntensity() {
  int rawValue = analogRead(RAIN_PIN);  // Read the rain sensor's raw value
  // Map raw value (0 = wet, 4000 = dry) to percentage (0% = wet, 100% = dry)
  int rainPercent = map(rawValue, 0, 4000, 100, 0);  // Invert it: 0% for wet, 100% for dry
  return rainPercent;
}

// Function to send the servo position and sensor data to Flask server
void sendData(int servoPos, float distance, int rainIntensity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Create JSON data
    String postData = "{\"servo_position\": " + String(servoPos) + 
                      ", \"distance\": " + String(distance) + 
                      ", \"rain_intensity\": " + String(rainIntensity) + "}";
    
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

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set up servo motor
  ESP32PWM::allocateTimer(0);
  myservo.setPeriodHertz(50);    
  myservo.attach(servoPin, 500, 2400);  // Attach the servo with pulse widths for 0° to 180°

  // Set up ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Set up rain sensor pin (analog input)
  pinMode(RAIN_PIN, INPUT);
}

void loop() {
  // Variables for sensor data
  float distance = getDistance();  // Get distance from ultrasonic sensor
  int rainIntensity = getRainIntensity();  // Get rain intensity as a percentage

  // Move the servo from 0° to 180°
  for (int pos = 0; pos <= 180; pos++) {
    myservo.write(pos);  // Move the servo to 'pos' degrees

    // Immediately update the distance reading and send data
    distance = getDistance();  // Recheck distance at each servo position
    sendData(pos, distance, rainIntensity);  // Send the servo position and sensor data to the server

    delay(5);  // Adjust the speed of the servo
  }
  
  delay(200);  // Shorter pause before reversing direction

  // Move the servo from 180° back to 0°
  for (int pos = 180; pos >= 0; pos--) {
    myservo.write(pos);  // Move the servo to 'pos' degrees

    // Immediately update the distance reading and send data
    distance = getDistance();  // Recheck distance at each servo position
    sendData(pos, distance, (100 - rainIntensity));  // Send the servo position and sensor data to the server

    delay(5);  // Adjust the speed of the servo
  }

  delay(200);  // Shorter pause before restarting the loop
}
