#include <SPI.h>
#include <EthernetV2_0.h>

byte            mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress       hueHubIP(127, 0, 0, 1); // Change to IP address of Hue Bridge
const char      hueUsername[] = "username"; // Change to Hue username
const int       hueHubPort = 80; // Hue Web Port
int             inputPin[] = { 5, 6, 7, 8 };
int             pinCount = 4;
int             pirState = LOW; // we start, assuming no motion detected
int             val = 0;  // variable for reading the pin status
boolean         hueOn;    // on/off
int             hueBri;   // brightness value
String          hueCmd;   // Hue command
int             timer = 0;
int             lightNum = 0;

// Ethernet
EthernetClient  client;
#define W5200_CS  10
#define SDCARD_CS 4


// the setup function runs once when you press reset or power the board
void setup()
{
    Serial.begin(9600);
    // pinMode(inputPin, INPUT);
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
  pinMode(inputPin[thisPin], INPUT);
    }
    pinMode(SDCARD_CS, OUTPUT);
    digitalWrite(SDCARD_CS, HIGH);  // Deselect the SD card
    while (!Serial) {
  ;     // wait for serial port to connect. Needed 
        // for Leonardo only
    }
    // start the Ethernet connection:
    if (Ethernet.begin(mac) == 0) {
  // Serial.println("Failed to configure Ethernet using DHCP");
  // no point in carrying on, so do nothing forevermore:
  for (;;);
    }
    // give the Ethernet shield a second to initialize
    // Serial.println("connected to web server");
    // Serial.println(Ethernet.localIP());
    // Serial.println(Ethernet.subnetMask());
    // Serial.println(Ethernet.gatewayIP());
    // Serial.println(Ethernet.dnsServerIP());
    // Serial.println("connecting...");
    // if you get a connection, report back via serial:
    if (client.connect(hueHubIP, 80)) {
  // Serial.println("connected to web server");
  // Make a HTTP request:
  client.println("GET / HTTP/1.0");
  client.println();
    } else {
  // kf you didn't get a connection to the server:
  // Serial.println("connection to web server failed");
    }
}

// httpRequest();

void loop()
{
    if (timer == 600) {
  // Serial.println("connected to web server");
  hueCmd = "{\"on\":false,\"transitiontime\":3000}";
  lightNum = 1;
  httpRequest(1);
  lightNum = 2;
  httpRequest(2);
  lightNum = 13;
   httpRequest(13);
  timer = 0;
  // Serial.println("hueCmd executed 2!");
    } else {
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
      // Serial.print("Timer : ");
      // Serial.println(timer);

      val = digitalRead(inputPin[thisPin]); // read input
              // value
      // Serial.print("Input : ");
      // Serial.print(inputPin[thisPin]);
      // Serial.print(" ");
      // Serial.println(val);
      if (val == HIGH) {  // check if the input is HIGH
    if (pirState == LOW) {
        // we have just turned on
        // Serial.print("Motion detected on ");
        // Serial.println(inputPin[thisPin]);
        // Serial.println("connected to web server");
        hueCmd = "{\"on\":true,\"bri\":254}";
        lightNum = 1; // 13
        httpRequest(1);
        // Serial.println("hueCmd executed 1!");
        lightNum = 2;
        httpRequest(2);
        // Serial.println("hueCmd executed 1!");
        lightNum = 13; // 13
        httpRequest(13);
        // Serial.println("hueCmd executed 1!");
        timer = 0;
        // We only want to print on the output change, not
        // state
        pirState = HIGH;
    }
      } else {
    if (pirState == HIGH) {
        // we have just turned off
        // Serial.println("Motion ended!");
        // We only want to print on the output change, not
        // state
        pirState = LOW;
    }
    timer++;
      }
  }
    }
}

int httpRequest(int lightNum)
{
    client.stop();
    client.connect(hueHubIP, 80);
    client.print("PUT /api/");
    client.print(hueUsername);
    client.print("/lights/");
    client.print(lightNum); // hueLight zero based, add 1
    client.println("/state HTTP/1.1");
    client.println("keep-alive");
    client.print("Host: ");
    client.println(hueHubIP);
    client.print("Content-Length: ");
    client.println(hueCmd.length());
    client.println("Content-Type: text/plain;charset=UTF-8");
    client.println();
    client.println(hueCmd);
    client.stop();
}

/* A helper function in case your logic depends on the current state of the light. 
 * This sets a number of global variables which you can check to find out if a light is currently on or not
 * and the hue etc. Not needed just to send out commands
 */
boolean getHue(int lightNum)
{
  if (client.connect(hueHubIP, hueHubPort))
  {
    client.print("GET /api/");
    client.print(hueUsername);
    client.print("/lights/");
    client.print(lightNum);  
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(hueHubIP);
    client.println("Content-type: application/json");
    client.println("keep-alive");
    client.println();
    while (client.connected())
    {
      if (client.available())
      {
        client.findUntil("\"on\":", "\0");
        hueOn = (client.readStringUntil(',') == "true");  // if light is on, set variable to true
 
        client.findUntil("\"bri\":", "\0");
        hueBri = client.readStringUntil(',').toInt();  // set variable to brightness value
        break;  // not capturing other light attributes yet
      }
    }
    client.stop();
    return true;  // captured on,bri,hue
  }
  else
    return false;  // error reading on,bri,hue
}
