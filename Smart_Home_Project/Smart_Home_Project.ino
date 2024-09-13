// For the WiFi connection 
#include <WiFi.h>
// The webserver
#include <WebServer.h>
// Librarys for DHT11 sensor
#include <DHT.h>
#include <DHT_U.h>
#include <DHT118266.h>
#include <Adafruit_Sensor.h>
// LCD 1602 Display Module...
#include <LiquidCrystal_I2C32.h>
// ...including I2C
#include <Wire.h>

/* DHT sensor settings */
#define DHTPIN 23 // Pin
#define DHTTYPE DHT11 // Type
DHT dht(DHTPIN, DHTTYPE);

/* LCD settings*/
// Address 0x27 for a 16x2 LCD 
#define LCD_ADDRESS 0x27  // I2C-Adresse für LCD (kann variieren)
#define LCD_COLUMNS 16
#define LCD_ROWS 2
LiquidCrystal_I2C32 lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// LED pin
const int ledPin = 13;

/* WiFi settings */
const char* ssid = "YourWiFiName"; // WiFi name
const char* password = "YourWiFiPassword"; //WiFi password

// Initialize the server
WebServer server(80); // Listening on standard port

// Threshold values for alarm
float tempThreshold = 25.0f;
float humThreshold = 60.0f;

void setup() {
  // Initialize I2C and set SDA and SCL pins 
  Wire.begin(21, 22);
  
  // Starts the serial communication between the device and the computer
  // 115200 bits per second
  Serial.begin(115200);

  // Starts the DHT sensor
  dht.begin();

  // Connecting the ESP32 with the WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Signals the waiting process for the WiFi connection
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    ++attempts;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("WiFi connection failed.");
  }

  // Defies a route for the webs server. Entering the ip
  // of the ESP32 calls the function handleRoot
  server.on("/", handleRoot);
  server.on("/set", handleSet);
  // Starts the web server
  server.begin();
  Serial.println("Server is running.");

  // Initializing LCD
  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  // Setting backlight
  lcd.backlight();

  // LED output
  pinMode(ledPin, OUTPUT);

}

void loop() {
  // Checking for new HTTP requests
  server.handleClient();

  // Updating LCD
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Error handling
  // Fehlerbehandlung
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading DHT11 sensors!");
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");
  
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print(" %");

  // LCD control based on threshold
  if (temperature > tempThreshold || humidity > humThreshold) {
    digitalWrite(ledPin, HIGH); // LED on
  } else {
    digitalWrite(ledPin, LOW);  // LED off
  }

  delay(2000); 
}

// Web server
void handleRoot() {
  // Reading the temperature and humidity values
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Creating an HTML page to display actual sensor values
  String html = "<html><head><title>Smart Home Control</title></head><body>";
  html += "<h1>Monitoring</h1>";
  html += "<p>Temperature: " + String(temperature) + " °C</p>";
  html += "<p>Humidity: " + String(humidity) + " %</p>";

 if (temperature > tempThreshold && humidity > humThreshold) {
    html += "<p style='color:red;'>Warning: Exceeded both threshold values!</p>";
} else {
    if (temperature > tempThreshold) {
        html += "<p style='color:red;'>Warning: Exceeded threshold for temperature!</p>";
    }
    if (humidity > humThreshold) {
        html += "<p style='color:red;'>Warning: Exceeded threshold for humidity!</p>";
    }
    if (temperature <= tempThreshold && humidity <= humThreshold) {
        html += "<p>Status: Normal</p>";
    }
}
   

  
  // Creates an HTML form where users can input new threshold values for temperature and humidity
  // The form sends a POST request to the /set URL when submitted, with the updated threshold values.
  html += "<form method='POST' action='set'>";
  html += "Threshold temperature: <input type='text' name='temp' value='" + String(tempThreshold) + "'><br>";
  html += "Threshold humidity: <input type='text' name='hum' value='" + String(humThreshold) + "'><br>";
  html += "<input type='submit' value='set'>";
  html += "</form>";

  html += "</body></html>";

  // Completes the HTML structure and sends it as a response with
  // a 200 status code (OK) and a response body which contains
  // HTML to the client requesting the page.
  server.send(200, "text/html", html);
}

// Changes the formula with regard to the new threshold values
void handleSet() {
  if(server.hasArg("temp") && server.hasArg("hum")) {
    // Both values (temp/hum) are called from the request
    // and saved.
    tempThreshold = server.arg("temp").toFloat();
    humThreshold = server.arg("hum").toFloat();
  }

  // Sends a HTTP header to the browser. After processing
  // the request the page is diverted to a new URL ("/").
  server.sendHeader("Location", "/");
  // HTTP request sends a status code which indicates a
  // diversion to another URL.
  server.send(303); 
}