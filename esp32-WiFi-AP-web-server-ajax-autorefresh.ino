/*
  ESP32 wifi Access point, Server, ajax, auto refresh, mDNS 
  This example shows how to:
  - set up an WiFi access point 
  - being able to connect using mDNS (connect through e.g. http://esp32-md.local)
  - Open and show a one page webserver
  - Request and GET an updated counter value through Ajax 
  - Perform automatically the update request every 500ms

  Steps:
  1. Connect to the access point "ESP32-AP" insert password "password"
  2. Point your web browser to http://192.168.4.1/ or http://esp32-md.local 
  3. it will start showing a value changing every 500ms. The background colour will change too as soon as a new value is ready

 
  Created by Marco Daldoss, 
  for arduino-esp32 on 16 Sept, 2021
  inspired by Elochukwu Ifediora (fedy0) and circuitsforyou.com
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "index.h"  //Web page header file
#include <ESPmDNS.h>


// Set these to your desired credentials.
const char *ssid = "ESP32-AP";
const char *password = "password";

WiFiServer server(80);

uint8_t counter = 0;

void handleAjaxReadData(WiFiClient client){
  counter++;
  String counterStr = String(counter);
  //client.println("HTTP/1.1 200 OK");
  //client.println("Content-type:text/html");
  client.println();
  client.print(counterStr);
  client.println();
}

 
void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  if (MDNS.begin("esp32-md")) {
    Serial.println("MDNS responder started. Navigate to http://esp32-md.local");
  }
  server.begin();

  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        
                // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /readData")) {
            handleAjaxReadData(client);
            break;
        }
        
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            String s = MAIN_page; //Read HTML contents
              // the content of the HTTP response follows the header:
            client.print(s);
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }


      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
