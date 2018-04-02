#define DEBUG_MODE true

#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GPS.h>

#define VBATPIN A9

// Initialize the OLED Display + Extras
Adafruit_SSD1306 display = Adafruit_SSD1306();
#define BUTTON_A 9
#define BUTTON_B 6
#define BUTTON_C 5
#define LED      13

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
File logfile;
#define LOG_SDCS 4

// Initialize the GPS Module

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

//Periodic (sleep) mode command for GPS module
//2 is periodic standby mode
//5000 is the active time in milliseconds
//15000 is the sleep time in milliseconds
//Leave the mode alone. Change the times to fit your needs
// REMEMBER to CHANGE the CHECKSUM
// Checksum calculator is here:
//  http://www.hhhh.org/wiml/proj/nmeaxor.html
#define PMTK_PERIODIC_MODE "$PMTK225,2,5000,15000,0,0*18"
//#define PMTK_PERIODIC_MODE_223 "$PMTK223,1,25,180000,60000*38"
#define PMTK_NORMAL_MODE "$PMTK225,0*2B"

uint32_t dataTimeStamp; //time of most recent data received from GPS. 
                        //  Sleep arduino when it's been more than 1 second
                        //  since the most recent data from GPS.

void setup() {
  
  // put your setup code here, to run once:
  if (DEBUG_MODE)
    while (!Serial);  // uncomment to have the sketch wait until Serial is ready

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(115200);
  GPS.begin(9600);

  setup_GPS();
  setup_Display();
  setup_IO();
  
}

void setup_GPS() {
  
  // Initialize GPS
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  //GPS.sendCommand(PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ); // 10 sec. update rate
  GPS.sendCommand(PGCMD_ANTENNA); // Request updates on antenna status
  GPS.sendCommand(PMTK_NORMAL_MODE);
}

void setup_Display() {

  // ...
}

void setup_IO() {
  // Initialize buttons from OLED Feather Wing
  // ...
}

void loop() {
  if (GPSSerial.available())
    GPS.read();
    
  parseNMEA();
}

void parseNMEA() {
  
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
    else
      Serial.println(GPS.lastNMEA());
  }
}

float getBatVoltage() {
  // gets the voltage at Pin D9/A7 and calculates the current battery status
  //...
}
