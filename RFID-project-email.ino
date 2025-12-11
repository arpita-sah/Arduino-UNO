/*
 * ESP32 + RFID (RC522) + Gmail Notification
 * Sends email when ANY of these two RFID tags are scanned:
 * 1) A1 10 20 1B
 * 2) A3 B5 CB 9A
 */

#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <SPI.h>
#include <MFRC522.h>

// WIFI 
#define WIFI_SSID "mbh"
#define WIFI_PASSWORD "001234mb"

// EMAIL
#define SENDER_EMAIL "roboticarpisah@gmail.com"
#define SENDER_PASSWORD "wfcw alzf tgyx wyyn"   // Google App Password
#define RECIPIENT_EMAIL "a.sah.7082@westcliff.edu"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 587

SMTPSession smtp;

//  RFID 
#define SS_PIN   21
#define RST_PIN  27

MFRC522 rfid(SS_PIN, RST_PIN);

// TWO Allowed RFID Tags (no spaces, uppercase)
String allowedTag1 = "A110201B";
String allowedTag2 = "A3B5CB9A";

void setup() {
  Serial.begin(9600);

  WiFi.begin("mbh", "001234mb");
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nWiFi Connected!");
  Serial.println(WiFi.localIP());

  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Scan your RFID tag...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  // Convert UID to string
  String uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uidString += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    uidString += String(rfid.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();

  Serial.print("Scanned UID: ");
  Serial.println(uidString);

  // CHECK ALLOWED TAGS
  if (uidString == allowedTag1 || uidString == allowedTag2) {
    Serial.println("AUTHORIZED TAG DETECTED!");

    String subject = "RFID Scan Notification - ESP32";
    String msg = "RFID Tag Scanned Successfully.\n";
    msg += "UID: " + uidString;

    gmail_send(subject, msg);
  } 
  else {
    Serial.println("Unauthorized tag scanned.");
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(1500);
}
// EMAIL FUNCTION
void gmail_send(String subject, String textMsg) {

  MailClient.networkReconnect(true);
  smtp.debug(1);
  smtp.callback(smtpCallback);

  Session_Config config;
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = SENDER_EMAIL;
  config.login.password = SENDER_PASSWORD;
  config.login.user_domain = F("127.0.0.1");
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 3;

  SMTP_Message message;
  message.sender.name = F("ESP32 RFID System");
  message.sender.email = SENDER_EMAIL;
  message.subject = subject;
  message.addRecipient(F("User"), RECIPIENT_EMAIL);

  message.text.content = textMsg;
  message.text.transfer_encoding = "base64";
  message.text.charSet = F("utf-8");

  if (!smtp.connect(&config)) {
    Serial.printf("SMTP Connection Error: %s\n", smtp.errorReason().c_str());
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.printf("Email Sending Error: %s\n", smtp.errorReason().c_str());
  }
}
void smtpCallback(SMTP_Status status) {
  Serial.println(status.info());
  if (status.success()) {
    Serial.printf("Emails Sent: %d\n", status.completedCount());
    smtp.sendingResult.clear();
  }
}
