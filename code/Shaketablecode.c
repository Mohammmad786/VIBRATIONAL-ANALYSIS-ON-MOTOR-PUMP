#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Meenarvi Anupam";
const char* password = "nimmi.2k";
const String apiKey = "5HH357RP3XAJ503G";  // Your ThingSpeak API Key
const char* server = "api.thingspeak.com";
const int field1 = 1;  // Field number for Vibration X
const int field2 = 2;  // Field number for Vibration Y
const int field3 = 3;  // Field number for Vibration Z

Adafruit_MPU6050 mpu;

void setup() {
  Wire.begin(21, 22);  // Initialize I2C communication with SDA on GPIO 21 and SCL on GPIO 22
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float vibrationX = a.acceleration.x;
  float vibrationY = a.acceleration.y;
  float vibrationZ = a.acceleration.z;

  // Get current timestamp in seconds
  unsigned long currentTimeSeconds = millis() / 1000;

  Serial.print("Time (s): ");
  Serial.print(currentTimeSeconds);
  Serial.print(" | Vibration (X, Y, Z): ");
  Serial.print(vibrationX);
  Serial.print(", ");
  Serial.print(vibrationY);
  Serial.print(", ");
  Serial.println(vibrationZ);

  // Send data to ThingSpeak
  sendDataToThingSpeak(currentTimeSeconds, vibrationX, vibrationY, vibrationZ);

  // Send data to PLX-DAQ
  sendDataToPLX_DAQ(currentTimeSeconds, vibrationX, vibrationY, vibrationZ);

  delay(10);  // Adjust the delay for your desired update rate
}

void sendDataToThingSpeak(unsigned long time, float x, float y, float z) {
  HTTPClient http;

  String url = String("http://") + server + "/update?api_key=" + apiKey +
               "&created_at=" + String(time) +
               "&field1=" + String(x) +
               "&field2=" + String(y) +
               "&field3=" + String(z);

  if (http.begin(url)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Data sent to ThingSpeak successfully");
    } else {
      Serial.println("Error sending data to ThingSpeak");
    }
    http.end();
  } else {
    Serial.println("Failed to connect to ThingSpeak");
  }
}

void sendDataToPLX_DAQ(unsigned long time, float x, float y, float z) {
  // Print the "DATA" label and the sensor data
  Serial.print("DATA,TIME,");
  Serial.print(time);
  Serial.print(",");
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.println(z);
}