// Import required libraries
#include "ESP8266WiFi.h"
#include "DHT.h"
#include <QueueArray.h>

// WiFi parameters
const char* ssid = "birds";
const char* password = "ceardida";

//String airQualityHistory = "";

//Sensor Setup
#define airquality_sensor_pin 0
#define gas_sensor_pin 1

// Pin
#define DHTPIN 5

// Use DHT11 sensor
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// Host
const char* host = "dweet.io";


  
void setup() {
  
  // Start Serial
  Serial.begin(115200);
  delay(10);
  
  // Init DHT 
  dht.begin();

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
}

void loop() {
 
  Serial.print("Connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // This will send the request to the server
  int airquality_value = analogRead(airquality_sensor_pin);
  /*if (airQualityHistory.length()==0)
    airQualityHistory = "[" + String(airquality_value);
  else
    airQualityHistory = airQualityHistory + "," + String(airquality_value);  */
  
  int gas_value = analogRead(gas_sensor_pin);
  float volume = (float)gas_value/1024*5.0*1000;
 
  delay(50);
  String message = String("GET /dweet/for/arisgassensor?airquality_value=") + String(airquality_value) + "&volume=" + String(volume) + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n";
  client.print(message);
  Serial.println(message);
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");


  
  // Repeat every 10 seconds
  delay(1000);
 
}

