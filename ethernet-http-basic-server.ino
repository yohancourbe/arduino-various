
//zoomkat 5-24-10
// http://www.arduino.cc/en/Tutorial/TextString for WString.h
#include <WString.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 0, 102 }; // ip in lan
byte gateway[] = { 192, 168, 0, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask

EthernetServer server(80); //server port

void setup() {

  //start Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

  //enable serial data print
  Serial.begin(9600);
}

void loop() {
  bool debug = true;

  // Create a client connection 
  EthernetClient client = server.available();
  
  String raw_request[50];
  String buffer = "";
  int line = 0;
  
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        
        char c = client.read();
  
        if (c == '\n') {

          if (debug) Serial.println("line ended: *" + buffer + "*");

          // if buffer only contains \r it's the end of the request
          if (buffer == "\r") {
            if (debug) Serial.println("request over");

            // Answer to client and close connection
            client.write("HTTP/1.1 200 OK");
            client.stop();

            if (debug) {
              for (int i = 0; i < line; i++){
                  Serial.println(raw_request[i]);
              }
            }

            // Analyse request, ignoring parameters for the moment

            // Split first line of request to get method and URI.
            String method = raw_request[0].substring(0, raw_request[0].indexOf(' '));
            String uri = raw_request[0].substring(raw_request[0].indexOf(' ') + 1, raw_request[0].indexOf(' ', raw_request[0].indexOf(' ') + 1));
            
            if (debug) Serial.println("Method: " + method);
            if (debug) Serial.println("URI: " + uri);

            // Implement logic based on URI or whatever, here I'll use simple URI because I want to use that for a specific usage, feel free to make it better
            if (uri == "/1") {
              Serial.println("push button 1");
            }
          }

          // Store buffer in request array and clear buffer
          raw_request[line] = buffer;
          line ++;
          buffer = "";

        } else {
          buffer += c;
        }
      }

      delay(10);
    }
  }

  delay(10);
}
