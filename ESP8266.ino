/*
 * Converted for ESP8266 (NodeMCU / Wemos LOLIN V3)
 * Original ESP32 version by esp32io.com
 */

// ESP8266: TCP SERVER + LED
#include <ESP8266WiFi.h>   // ESP8266 WiFi library

#define LED_PIN  D4        // Built-in LED on most ESP8266 boards (GPIO2)
#define SERVER_PORT 6000

const char* ssid = "mbh";              // Your WiFi SSID
const char* password = "001234mb"; // Your WiFi Password

WiFiServer TCPserver(SERVER_PORT);

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // LED OFF (active-LOW on ESP8266)

  Serial.println("ESP8266: TCP SERVER + LED");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi");

  // Print local IP address
  Serial.print("ESP8266 TCP Server IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Use this IP in the ESP32 #1 client code");

  // Start the server
  TCPserver.begin();
}

void loop() {
  WiFiClient client = TCPserver.available();
  delay(100);

  if (client) {
    // Read 1 command from client
    char command = client.read();
    Serial.print("Received command: ");
    Serial.println(command);

    if (command == '0') {
      digitalWrite(LED_PIN, LOW); // ESP8266 LED ON
    }
    else if (command == '1') {
      digitalWrite(LED_PIN, HIGH); // ESP8266 LED OFF
    }

    client.stop();
  }
}
