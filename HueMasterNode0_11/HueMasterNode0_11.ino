/*
    Begin includes for Wifi, I2C, LCD
    Timer, Time and Free Memory
*/

// #include "Arduino.h"
#include<CountUpDownTimer.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Time.h>
#include <MemoryFree.h>

/*
 * LCD Init of Pins to LCD Pins
 * RS, EN, D4. D5, D6, D7
 */
// LiquidCrystal lcd(4, 15, 16, 17, 8, 9);
LiquidCrystal lcd(18, 19, 14, 15, 16, 17);

/*
 * Setup Countdown Timer
 */

CountUpDownTimer T(DOWN);

/*
 *
 */

byte            pirState = LOW; // we start, assuming no motion detected
byte            webCommandSent = LOW;
byte            val = 0;  // variable for reading the pin status
byte            remotePIRValue;
int             c;
unsigned long   lastRead = millis();
byte            lightPin = A0;
int             receivedValue;
byte            EDT = 0;
// int             valLDR;
// bool            gotValidIP;
const unsigned long
connectTimeout  = 15L * 1000L, // Max time to wait for server connection
responseTimeout = 15L * 1000L; // Max time to wait for data from server
unsigned long int
countdown       = 0;  // loop() iterations until next time server query
unsigned long
lastPolledTime  = 0L, // Last value retrieved from time server
sketchTime      = 0L; // CPU milliseconds since last server query

byte            inputPin[] = {6, 7};
byte            pinCount = 2;

#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
//                     MOSI = 11
//                     MISO = 12
//                      SCK = 13
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                         SPI_CLOCK_DIVIDER); // you can change this clock speed
Adafruit_CC3000_Client client;

const char * WLAN_SSID  =    "SSID"; // cannot be longer than 32 characters! Change to real SSID
const char  * WLAN_PASS  =    "PSK"; // Change to real PreShareKey    
#define WLAN_SECURITY   WLAN_SEC_WPA2 // WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define IDLE_TIMEOUT_MS 3000      // Amount of time to wait (in milliseconds) with no data
const char * WEBSITE  =   "10.0.0.107"; // Change to Hue Bridge IP Address
const char      hueUsername[] = "username"; // Change to Hue username
const byte      hueHubPort = 80; // Hue Web Port
uint32_t        ip;
String          hueCmd;   // Hue command
byte            lightNum = 13;

/*
 *
 */

void setup(void)
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(F("Initializing"));
  Serial.begin(115200);
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.println(F("Starting..."));
  T.SetTimer(0, 5, 0);
  T.StartTimer();
  pinMode(lightPin, INPUT);
  // Initialize the Input Pins 6-9
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(inputPin[thisPin], INPUT);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Hello, CC3000"));

  Serial.println(F("\nInitialising the CC3000 ..."));
  /* Initialise the module */
  if (!cc3000.begin())
  {
    Serial.println(F("Unable to initialise the CC3000! Check your wiring?"));
    while (1);
  }


connectToWifi();


  ip = 0;
  // Try looking up the website's IP address
  while (ip == 0) {
    if (! cc3000.getHostByName(WEBSITE, &ip)) {
    }
    delay(500);
  }

  cc3000.printIPdotsRev(ip);
  Serial.println();
  lcd.clear();
}

void loop(void)
{
   if (countdown == 0) {           // Time's up?
    unsigned long t  = getTime(); // Query time servercc3000 connecttcp
    if (t) {                      // Success?
      lastPolledTime = t;         // Save time
      sketchTime     = millis();  // Save sketch time of last valid time query
      countdown      = 1000L * 24 * 60 * 4 - 1; // Reset counter: 24 hours * 15-second intervals
    }
  } else {
    countdown--;                  // Don't poll; use math to figure current time
  } // END if | else for countdown

  T.Timer(); // run the timer
    remotePIRValue = GetSensePinValue(1); // 1 - slave's address, reading data sent from slave PIR
  // valLDR = analogRead(lightPin); // reading lightPin LDR, used to decide whether to arm PIR's
  // valLDR = map(valLDR, -0, 985, 0, 255); // map analogRead to 0 to 255
  if (T.TimeCheck(0, 0, 0)) {
    hueCmd = "{\"on\":false,\"transitiontime\":300}";
    putHueLightState(13);
    // delay(10);
    putHueLightState(14);
    // delay(10);
    putHueLightState(1);
    T.SetTimer(0, 0, 30);
    T.StopTimer();
    pirState = 0;
    webCommandSent = 0;
  } else {
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
      val = digitalRead(inputPin[thisPin]); // read input valuecc3000 connecttcp
      if (val == HIGH) {  // check if the input is HIGH
        if (pirState == 0) {
        	if (webCommandSent ==0){
        		Serial.print(F("LOCAL Sensor FIRING!!"));
        		        		hueCmd = "{\"on\":true,\"bri\":254}";
        		putHueLightState(13);
        		putHueLightState(14);
        		putHueLightState(1);
        		webCommandSent = 1;
        	}
        	T.SetTimer(0, 5, 0);
        	pirState = 1;
        } // END if statement that evaluates val as HIGH and sets pirstate as HIGH, turns on lights and resets timer
      } else {
        pirState = 0;
      } // END if else that evaluates val
    } // END for lop that digitalReads input pinsvalLDR
    if (remotePIRValue == 255) {
        if (pirState == 0) {
            if (webCommandSent ==0){
            	hueCmd = "{\"on\":true,\"bri\":254}";
            	Serial.print(F("Remote Sensor FIRING!!"));
            	putHueLightState(13);
            	putHueLightState(14);
            	putHueLightState(1);
            	webCommandSent = 1;
            }
            T.SetTimer(0, 5, 0);
            pirState = 1;
        }
    } else {
           pirState = 0;
          }

  } //END if|else for TimeCheck 0,0,0
  unsigned long currentTime = lastPolledTime + (millis() - sketchTime) / 1000;
  setTime(currentTime);
  showLCDTime();
 serialDebugPrint();
 }  // END void loop()

/*
 *  Moved cc3000 connection to WiFi to function to clean up void setup
 */

 void connectToWifi(void)
 {
   connectToWifiAndGetIP:
  /* Attempt to connect to an access point */
  // char * ssid = WLAN_SSID;             /* Max 32 chars */cc3000 connecttcp
//  Serial.print(F("\nAttempting to connect to ")); Serial.println(ssid);
  Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Connecting to"));
  lcd.setCursor(0, 1);
//  lcd.print(ssid);
  /* NOTE: Secure connections are not available in 'Tiny' mode! */
  lcd.print(WLAN_SSID);

  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Connect Failed!"));
    while (1);
  }

  Serial.println(F("Connected!"));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Connected!"));
  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Request DHCP..."));

  while (!cc3000.checkDHCP())
  {
    delay(100); // Insert a DHCP timeout!
  }
  while (!displayConnectionDetails()) {

    delay(1000);
  }
  if (displayConnectionDetails() == FALSE) {
    cc3000.disconnect();
    delay(5000);
    cc3000.begin();
    goto  connectToWifiAndGetIP;
  } else {
    return;
  }

  // return cc3000.connected();
 }

/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if (!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}
/*
   Get Light status from Hue Bridge
*/

void getHueLightState(int lightNum)
{
  Adafruit_CC3000_Client www = cc3000.connectTCP(ip, hueHubPort);
  Serial.println(F("Getting Light State"));
  if (www.connected()) {
    Serial.println(F("Connected"));
    www.fastrprint(F("GET "));
    www.fastrprint("/api/");
    www.fastrprint(hueUsername);
    www.fastrprint("/lights/");
    www.print(lightNum);
    www.fastrprint("/state/");
    www.fastrprint(F(" HTTP/1.1\r\n"));
    www.fastrprint(F("Host: ")); www.fastrprint(WEBSITE); www.fastrprint(F("\r\n"));
    www.fastrprint(F("\r\n"));
    www.println();
    Serial.println(F("Request Sent!!!"));
  } else {
    return;
  }
  /* Read data until either the connection is closed, or the idle timeout is reached. */
  while (www.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
    while (www.available()) {
      char c = www.read();
      Serial.print(c);
      lastRead = millis();
    }
  }
  www.close();
}

/*cc3000 connecttcp
   Put Light state to Hue Bridge
*/

void putHueLightState(int lightNum)
{
  Adafruit_CC3000_Client www = cc3000.connectTCP(ip, hueHubPort);
  Serial.println(F("PUT Connecting"));
  if (www.connected()) {
    Serial.println(F("Connected"));
    www.fastrprint(F("PUT /api/"));
    www.fastrprint(hueUsername);
    www.fastrprint(F("/lights/"));
    www.print(lightNum);
    www.fastrprint(F("/state/ HTTP/1.1\r\nkeep-alive\r\nHost: "));
    www.fastrprint(WEBSITE);
    www.fastrprint(F("\r\nContent-Length: "));
    www.print(hueCmd.length());
    www.fastrprint(F("\r\nContent-Type: text/plain;charset=UTF-8\r\n\r\n"));
    www.print(hueCmd);
    www.print(F("\r\n\r\n"));
    Serial.print("PUT: ");
    Serial.print(hueCmd);
    Serial.println(F("Request Sent!!"));
  } else {
    return;
  }
  while (www.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
    while (www.available()) {
      char c = www.read();
      Serial.print(c);
      lastRead = millis();
    }
  }
  www.close();
}

/*
   Read I2C data from slave PIR sensor
*/

int GetSensePinValue(byte SlaveDeviceId) {
  // SEND COMMAND
  Wire.beginTransmission(SlaveDeviceId);
  Wire.write(1); // Transfer command ("1") to get sensor value;
  delay(10);
  // GET RESPONSE
    int available = Wire.requestFrom(SlaveDeviceId, (byte)2);
  // Serial.print(F("Available: "));
  Serial.print(available);
  if (available == 2)
  {
    receivedValue = Wire.read() << 8 | Wire.read(); // combine two bytes into integer
    Serial.print(" | ");
    // Serial.print(F("RCVD: "));
    Serial.print(receivedValue);
  }
  else
  {
    Serial.print(F("ERROR: Unexpected number of bytes received (SensePinValue): "));
    Serial.println(available);
  }
  Wire.endTransmission();
  Serial.print(F(" | "));
  return receivedValue;
}

/*
    Serial Debug Output of variables for testing purposes
*/

void serialDebugPrint() {
  // Serial.print(F(" Connected status:"));
  Serial.print(cc3000.checkConnected());
  Serial.print(F(" | "));
  Serial.print(T.ShowMinutes());
  Serial.print(F(":"));
  Serial.print(T.ShowSeconds());
  Serial.print(F(" | "));
  // Serial.print(F("x: "));
  Serial.print(remotePIRValue);
  Serial.print(F(" | "));
  // Serial.print(F("valLDR: "));
  // Serial.print(valLDR);
  // Serial.print(F(" | "));
  Serial.print(hueCmd);
  Serial.print(F(" | "));
  // Serial.print(F("val: "));
  Serial.print(val);
  Serial.print(F(" | "));
  // Serial.print(F("Rpir: "));
  Serial.print(remotePIRValue);
  // Serial.print(F(" | pir: "));
  Serial.print(pirState);
  Serial.print(F(" | "));
  // Serial.print(F("freeMemory()="));
  Serial.println(freeMemory());
 }

/*
   Minimalist time server query; adapted from Adafruit Gutenbird sketch,
   which in turn has roots in Arduino UdpNTPClient tutorial.
*/

unsigned long getTime(void) {

  uint8_t       buf[48];
  unsigned long ip, startTime, t = 0L;

  Serial.print(F("Locating NTP..."));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Locating time server..."));
  // Hostname to IP lookup; use NTP pool (rotates through servers)
  if (cc3000.getHostByName("pool.ntp.org", &ip)) {
    static const unsigned char PROGMEM
    timeReqA[] = { 227,  0,  6, 236 },
                 timeReqB[] = {  49, 78, 49,  52 };

    Serial.println(F("\r\nAttempting connection..."));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Connecting NTP..."));
    startTime = millis();
    do {
      client = cc3000.connectUDP(ip, 123);
    } while ((!client.connected()) &&
             ((millis() - startTime) < connectTimeout));

    if (client.connected()) {
      Serial.print(F("connected!\r\nIssuing request..."));
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Requesting time..."));

      // Assemble and issue request packet
      memset(buf, 0, sizeof(buf));
      memcpy_P( buf    , timeReqA, sizeof(timeReqA));
      memcpy_P(&buf[12], timeReqB, sizeof(timeReqB));
      client.write(buf, sizeof(buf));

      Serial.print(F("\r\nAwaiting response..."));
      memset(buf, 0, sizeof(buf));
      startTime = millis();
      while ((!client.available()) &&
             ((millis() - startTime) < responseTimeout));
      if (client.available()) {
        client.read(buf, sizeof(buf));
        t = (((unsigned long)buf[40] << 24) |
             ((unsigned long)buf[41] << 16) |
             ((unsigned long)buf[42] <<  8) |
             (unsigned long)buf[43]) - 2208988800UL;
        Serial.print(F("OK\r\n"));
      }
      client.close();

    }
  }Serial.print(F(" | "));
  if (!t) Serial.println(F("error"));
  return (t);
}

/*
   Writing Time, LDR, MTN and SNS data to LCD screen
*/

void showLCDTime()
{
  if (hour() <= 4) {
    EDT = (hour() + 19);
  } else {
    EDT = (hour() - 5);
  }
  lcd.setCursor(0, 0);
  if (EDT < 10) {
    lcd.print(F("0"));
    }
    lcd.print(EDT);
    lcd.print(F(":"));
  if (minute() < 10) {
    lcd.print(F("0"));
    }
    lcd.print(minute());
  if (EDT < 12) {
    lcd.print(F("AM"));
  } else {
    lcd.print(F("PM"));
  }
  lcd.setCursor(0, 1);
  lcd.print(T.ShowMinutes());
    lcd.print(F(":"));
    if (T.ShowSeconds() < 10) {
        lcd.print(F("0"));
    }
    lcd.print(T.ShowSeconds());
  /* lcd.print(F("LDR:"));
  if (valLDR < 100) {
    lcd.print(F("0"));
  }
  if (valLDR < 10) {
    lcd.print(F("0"));
  }
  lcd.print(valLDR); */
  lcd.setCursor(7,0);
  lcd.print(F("  "));
  /* lcd.setCursor(9, 1);
  lcd.print(F("MTN:"));
  if (val == HIGH)  {
    lcd.print(F(" ON"));
  } else {
    lcd.print(F("OFF"));
  } */
  lcd.setCursor(9, 0);
  lcd.print(F("MTN:"));
  if (pirState == 1) {
    lcd.print(F(" ON"));
  } else  {
    lcd.print(F("OFF"));
  }
}
