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
const char* ssid = "yourwifiname"; // WiFi name
const char* password = "yourpassword"; //WiFi password

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
  // Connecting the ESP32 with the WiFi
  WiFi.begin(ssid, password);

  // Signals the waiting process for the WiFi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Defies a route for the webs server. Entering the ip
  // of the ESP32 calls the function handleRoot
  server.on("/", handleRoot);
  // Starts the web server
  server.begin();

}

void loop() {
  // Checking for new HTTP requests
  server.handleClient();
}

// Web server
void handleRoot() {
  // Reading the temperature and humidity values
  float temperature = dht.readTemperature;
  float humidity = dht.readHumidity;

  // Creating an HTML page to display actual sensor values
  String html = "<html><head><title>Smart Home Control</title></head><body>";
  html += "<h1>Monitoring</h1>";
  html += "<p>Temperature: " + String(temperature) + " °C</p>";
  html += "<p>Humidity: " + String(humidity) + " %</p>";

  if(temperature > )

  html += "<form method='POST' action='set'>";
  html += "Threshold temperature: <input type='text' name='temp' value='" + String(tempThreshold) + "'><br>";
  html += "Threshold humidity: <input type='text' name='hum' value='" + String(humThreshold) + "'><br>";
  html += "<input type='submit' value='set'>";
  html += "</form>";

}