// For the WiFi connection 
#include <esp_wifi.h>
// The webserver
#include <WebServer.h>
// Librarys for DHT11 sensor
#include <DHT.h>
#include <Adafruit_Sensor.h>

/* DHT sensor settings */
#define DHTPIN 23 // Pin
#define DHTTYPE DHT11 // Type
DHT dht(DHTPIN, DHTTYPE);

/* WiFi settings */
const char* ssid = "FRITZ!Box 7530 UR"; // WiFi name
const char* password = "73905551489688924355"; //WiFi password

// Initialize the server
WebServer server(80); // Listening on standard port

// Threshold values for alarm
float tempThres = 25.0f;
float humThresh = 60.0f;

void setup() {
  // Starts the serial communication between the device and the computer
  // 115200 bits per second
  Serial.begin(115200);
  // Starts the DHT sensor
  dht.begin;

}

void loop() {
  // put your main code here, to r:

}
