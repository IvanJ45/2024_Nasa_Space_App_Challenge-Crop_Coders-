"""
Author: Ivan Jovanovski
Date: October 6, 2024
Description: Flask server to handle sensor data submission from two ESP32 devices 
             (temperature, humidity, soil moisture, distance, rain intensity, and servo position) 
             and display the data on a web interface.
"""
from flask import Flask, jsonify, render_template, request

app = Flask(__name__)

# Global variable to store sensor data from both ESP32 boards
sensor_data = {
    "temperature": 0,   # Temperature from first ESP32
    "humidity": 0,      # Humidity from first ESP32
    "soil_moisture": 0, # Soil moisture from first ESP32
    "distance": 0,      # Distance from second ESP32
    "rain_intensity": 0,# Rain intensity from second ESP32
    "servo_position": 0 # Servo position from second ESP32
}

# Route to handle data submission from ESP32s
@app.route('/submit_data', methods=['POST'])
def submit_data():
    global sensor_data
    data = request.json
    if "temperature" in data:  # Check if temperature, humidity, soil moisture data is provided (from first ESP32)
        sensor_data['temperature'] = data['temperature']
        sensor_data['humidity'] = data['humidity']
        sensor_data['soil_moisture'] = data['soil_moisture']
    if "distance" in data:  # Check if distance and rain intensity data is provided (from second ESP32)
        sensor_data['distance'] = data['distance']
        sensor_data['rain_intensity'] = data['rain_intensity']
    if "servo_position" in data:  # Check if servo position data is provided (from second ESP32)
        sensor_data['servo_position'] = data['servo_position']
    return jsonify({"status": "Data received successfully!"}), 200

# Route to display the data on the web interface
@app.route('/')
def dashboard():
    return render_template('dashboard.html', sensor_data=sensor_data)

# Route to return sensor data for updates
@app.route('/get_data')
def get_data():
    return jsonify(sensor_data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
