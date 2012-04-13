/*
  Web Server
 
 A simple web server that turns pin 8 on when it recieves
 a GET request to `/on` and turns that pin off when
 it receives a GET request to `/off`.

 It also sends GET requests to '/heartbeat' for monitoring.
 
 by Chris Continanza
 
 */

#include <SPI.h>
#include <Ethernet.h>


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10,10,10,102);
IPAddress heartbeat_server(10,10,10,105); 
int heartbeat_port = 3000;
int heartbeat_delay_millis = 5000;

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

int LED = 8;
int last_time = 0;

void setup()
{
  pinMode(LED, OUTPUT);
 
  // open the serial port
  Serial.begin(9600);
  
  /*
 // Not using DHCP so we have a static connection
  // start the Ethernet connection:
  Serial.println("Trying to get an IP address using DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  */
  

  // connect w. static ip
  Ethernet.begin(mac, ip);

  
  
  // print your local IP address:
  Serial.print("My IP address: ");
  IPAddress my_ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(my_ip[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
  
  // start listening for clients
  server.begin();
}

void loop()
{
  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    
    //string for fetching data from address
    String httpRequest = "";
    
    while (client.connected()) {
      if (client.available()) {
         char c = client.read();
         httpRequest.concat(c);
         
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          Serial.print(httpRequest);
    
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          
          if(httpRequest.startsWith("GET /on")){
            Serial.println("LED ON");
            digitalWrite(LED, HIGH);   // set the LED on
            client.print("<h1>Outage Lights On</h1>");
          }else if(httpRequest.startsWith("GET /off")){
            Serial.println("LED OFF");
            digitalWrite(LED, LOW);    // set the LED off 
            client.print("<h1>Outage Lights Off</h1>");
          }
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
        
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
  
  //comment out if you don't want a heartbeat
  EthernetClient heartbeat;
  int time = millis();
  if((time % heartbeat_delay_millis < 10) && 
     (time - last_time > heartbeat_delay_millis)){
    Serial.println("heartbeat");  
    last_time = time;
    if (heartbeat.connect(heartbeat_server, heartbeat_port)) {
       Serial.println("connected");
       // Make a HTTP request:
       heartbeat.println("GET /heartbeat HTTP/1.0");
       heartbeat.println();
     }
     else {
       // kf you didn't get a connection to the server:
       Serial.println("connection failed");
     }
     heartbeat.flush();
     heartbeat.stop();
  }
}

