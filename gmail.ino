/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-gmail
 */

#include <WiFi.h>
#include <ESP_Mail_Client.h>
 
#define WIFI_SSID "mbh" // CHANGE IT
#define WIFI_PASSWORD "001234mb" // CHANGE IT

// the sender email credentials
#define SENDER_EMAIL "roboticarpisah@gmail.com" // CHANGE IT
#define SENDER_PASSWORD "wfcw alzf tgyx wyyn"  // CHANGE IT to your Google App password

#define RECIPIENT_EMAIL "a.sah.7082@westcliff.edu" // CHANGE IT

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 587

SMTPSession smtp;

void setup() {
  Serial.begin(9600);
  WiFi.begin(mbh, 001234mb);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  String subject = "Email Notification from ESP32";


  String textMsg = "This is an email sent from ESP32.\n";
  textMsg += "Sensor value: ";
  textMsg += "15"; // OR replace this value read from a sensor

  gmail_send(subject, textMsg);
}

void loop() {
  // YOUR OTHER CODE HERE
}

void gmail_send(String subject, String textMsg) {
  // set the network reconnection option
  MailClient.networkReconnect(true);

  smtp.debug(1);

  smtp.callback(smtpCallback);
  Session_Config config;

  // set the session config
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = SENDER_EMAIL;
  config.login.password = SENDER_PASSWORD;
  config.login.user_domain = F("127.0.0.1");
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 3;
  config.time.day_light_offset = 0;

  // declare the message class
  SMTP_Message message;

  // set the message headers
  message.sender.name = F("ESP32");
  message.sender.email = SENDER_EMAIL;
  message.subject = subject;
  message.addRecipient(F("To Whom It May Concern"), RECIPIENT_EMAIL);

  message.text.content = textMsg;
  message.text.transfer_encoding = "base64";
  message.text.charSet = F("utf-8");
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;

  // set the custom message header
  message.addHeader(F("Message-ID: <abcde.fghij@gmail.com>"));

  // connect to the server
  if (!smtp.connect(&config)) {
    Serial.printf("Connection error, Status Code: %d, Error Code: %d, Reason: %s\n", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }

  if (!smtp.isLoggedIn()) {
    Serial.println("Not yet logged in.");
  } else {
    if (smtp.isAuthenticated())
      Serial.println("Successfully logged in.");
    else
      Serial.println("Connected with no Auth.");
  }

  // start sending Email and close the session
  if (!MailClient.sendMail(&smtp, &message))
    Serial.printf("Error, Status Code: %d, Error Code: %d, Reason: %s\n", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
}

// callback function to get the Email sending status
void smtpCallback(SMTP_Status status) {
  // print the current status
  Serial.println(status.info());

  // print the sending result
  if (status.success()) {
    Serial.println("----------------");
    Serial.printf("Message sent success: %d\n", status.completedCount());
    Serial.printf("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");

    for (size_t i = 0; i < smtp.sendingResult.size(); i++) {
      // get the result item
      SMTP_Result result = smtp.sendingResult.getItem(i);

      Serial.printf("Message No: %d\n", i + 1);
      Serial.printf("Status: %s\n", result.completed ? "success" : "failed");
      Serial.printf("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
      Serial.printf("Recipient: %s\n", result.recipients.c_str());
      Serial.printf("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

    // free the memory
    smtp.sendingResult.clear();
  }
}
