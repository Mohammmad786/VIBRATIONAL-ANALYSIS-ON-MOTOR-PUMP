/*
  WriteMultipleFields
  
  Description: Writes values to fields 1,2,3,4 and status in a single ThingSpeak update every 20 seconds.
  
  Hardware: ESP32 based boards
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires installation of EPS32 core. See https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md for details. 
  - Select the target hardware from the Tools->Board menu
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

Adafruit_MPU6050 mpu1;
Adafruit_MPU6050 mpu2;

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;


String myStatus = "";

void setup() {
  Serial.begin(115200);  //Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  mpu1.begin(0x68);
  mpu2.begin(0x69);
  
  mpu1.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu2.setAccelerometerRange(MPU6050_RANGE_16_G);


  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }


  sensors_event_t a1, g1, temp1;
  mpu1.getEvent(&a1, &g1, &temp1);

  float vibrationX1 = a1.acceleration.x;
  float vibrationY1 = a1.acceleration.y;
  float vibrationZ1 = a1.acceleration.z;

  Serial.print("MPU1 - Vibration (X, Y, Z): ");
  Serial.print(vibrationX1);
  Serial.print(", ");
  Serial.print(vibrationY1);
  Serial.print(", ");
  Serial.println(vibrationZ1);

  // Read from MPU2
  sensors_event_t a2, g2, temp2;
  mpu2.getEvent(&a2, &g2, &temp2);

  float vibrationX2 = a2.acceleration.x;
  float vibrationY2 = a2.acceleration.y;
  float vibrationZ2 = a2.acceleration.z;

  Serial.print("MPU2 - Vibration (X, Y, Z): ");
  Serial.print(vibrationX2);
  Serial.print(", ");
  Serial.print(vibrationY2);
  Serial.print(", ");
  Serial.println(vibrationZ2);

  // set the fields with the values
  ThingSpeak.setField(1, vibrationX1);
  ThingSpeak.setField(2, vibrationY1);
  ThingSpeak.setField(3, vibrationZ1);
  ThingSpeak.setField(4, vibrationX2);
  ThingSpeak.setField(5, vibrationY2);
  ThingSpeak.setField(6, vibrationZ2);
  

  
  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  
  delay(15000); // Wait 20 seconds to update the channel again
}
