/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/communication-between-two-esp32
 */

// ESP32: TCP CLIENT + A BUTTON/SWITCH
#include <ezButton.h>
#include <WiFi.h>

#define BUTTON_PIN 21 // ESP32 pin GPIO21 connected to button


const char* ssid = "mbh";     // CHANGE TO YOUR WIFI SSID
const char* password = "001234mb"; // CHANGE TO YOUR WIFI PASSWORD
const char* serverAddress = "192.168.1.164"; // CHANGE TO ESP32#2'S IP ADDRESS
const int serverPort = 6000;
ezButton button(BUTTON_PIN); // create ezButton

WiFiClient TCPclient;

void setup() {
  Serial.begin(9600);
  button.setDebounceTime(50); // set debounce time to 50 milliseconds

  Serial.println("ESP32: TCP CLIENT + A BUTTON/SWITCH");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // connect to TCP server (Arduino #2)
  if (TCPclient.connect(serverAddress, serverPort)) {
    Serial.println("Connected to TCP server");
  } else {
    Serial.println("Failed to connect to TCP server");
  }
}

void loop() {
  button.loop(); // MUST call the loop() function first

  if (!TCPclient.connected()) {
    Serial.println("Connection is disconnected");
    TCPclient.stop();

    // reconnect to TCP server (Arduino #2)
    if (TCPclient.connect(serverAddress, serverPort)) {
      Serial.println("Reconnected to TCP server");
    } else {
      Serial.println("Failed to reconnect to TCP server");
    }
  }

  if (button.isPressed()) {
    TCPclient.write('1');
    TCPclient.flush();
    Serial.println("- The button is pressed, sent command: 1");
  }

  if (button.isReleased()) {
    TCPclient.write('0');
    TCPclient.flush();
    Serial.println("- The button is released, sent command: 0");
  }
  delay(100);
}
