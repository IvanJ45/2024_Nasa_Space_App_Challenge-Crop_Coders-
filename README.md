ESP32 Sensor Dashboard
Description

This project involves using two ESP32 devices to collect environmental data and control a servo motor. One ESP32 gathers temperature, humidity, and soil moisture data, while the second ESP32 monitors rain intensity and distance using an ultrasonic sensor. The second device also controls a servo motor, providing radar-like visualization. All the data is sent to a Python Flask web server, where a live dashboard visualizes the information, including a radar display.
Features

    Real-time data visualization: Temperature, humidity, soil moisture, rain intensity, and distance measurements.
    Radar-like display: Visualizes the servo motor's movement and corresponding distance readings.
    Wi-Fi enabled: Both ESP32 devices send sensor data via Wi-Fi to the Flask server.
    Multiple sensor support: Integrates several sensors (DHT22, ultrasonic, and rain sensor) for a comprehensive monitoring system.

Project Structure
ESP32 Code

Each ESP32 board runs its own Arduino sketch:

    ESP32 Board 1: Collects temperature, humidity, and soil moisture data.
    ESP32 Board 2: Measures distance (via ultrasonic sensor), rain intensity, and controls the servo motor.

Flask Server

A Python Flask server is used to:

    Handle incoming sensor data via POST requests.
    Serve the web dashboard, which displays live sensor readings.
    Visualize radar-like display movements and track the last 5 distance measurements.

Web Dashboard

The dashboard, built with HTML, CSS, and JavaScript, includes:

    Radar UI: Visual representation of servo movement and distance readings.
    Sensor data display: Real-time updates of temperature, humidity, soil moisture, and rain intensity.
    Trail visualization: Tracks and fades out the last 5 distance points for enhanced visualization.

Setup Instructions
ESP32 Devices

    Install the required libraries for Arduino IDE:
        ESP32Servo
        WiFi
        HTTPClient
    Flash the two ESP32 devices with the respective sketches:
        Board 1: Temperature, humidity, and soil moisture collection.
        Board 2: Rain intensity, distance measurement, and servo control.
    Modify the Wi-Fi credentials in each sketch:

    cpp

    const char* ssid = "your_SSID";
    const char* password = "your_PASSWORD";

Flask Server

    Install the required Python libraries:

    bash

pip install Flask

Run the Flask server:

bash

    python app.py

Web Dashboard

    Ensure the Flask server is running.
    Access the web dashboard by navigating to http://<ESP32_IP>:5000 in your browser.

Hardware Setup

    ESP32 Board 1:
        DHT22 (Temperature & Humidity Sensor)
        Soil Moisture Sensor
    ESP32 Board 2:
        Ultrasonic Sensor (for distance)
        Rain Sensor (analog input)
        Servo Motor (connected to GPIO 15)

Future Enhancements

    Add additional sensor types to enhance environmental monitoring capabilities.
    Enable data logging to track sensor readings over time.
