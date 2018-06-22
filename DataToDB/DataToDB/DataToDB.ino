#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "GravitySensorHub.h"
#include "GravityRtc.h"
#include "OneWire.h"
#include "SdService.h"
#include "Debug.h"
#include <TinyGPS++.h>

/* WaterMonitor */

// clock module
GravityRtc rtc;

// sensor monitor
GravitySensorHub sensorHub;
SdService sdService = SdService(sensorHub.sensors);

unsigned long updateTime = 0;

/* GPS */

#define GPS Serial2
TinyGPSPlus gpsParser;
double GPS_lat = 0.0;
double GPS_lng = 0.0;

/* ESP8266 */

#define ESP Serial3

/* Main */

#define UPDATE_TIME 20000

void setup() {
  Serial.begin(57600);

  // WaterMonitor
  sensorHub.setup();
  sdService.setup();

  // GPS
  GPS.begin(9600);

  // ESP8266
  ESP.begin(115200);
}

void loop() {
	sensorHub.update();
	sdService.update();

  if (GPS.available()) {
      char c = GPS.read();
      if (gpsParser.encode(c)) parseGPS();
  }

  if(millis() - updateTime > UPDATE_TIME)
  {
    updateTime = millis();
    sendData();
  }
}

void parseGPS() {
  if (gpsParser.location.isValid()) {
    GPS_lat = gpsParser.location.lat();
    GPS_lng = gpsParser.location.lng();
  }
}

void sendData() {
  ESP.print("{");
  
  ESP.print(F("\"pH\":  "));
  ESP.print(sensorHub.getValueBySensorNumber(0));
  ESP.print(",");
  
  ESP.print(F("\"Temperature\": "));
  ESP.print(sensorHub.getValueBySensorNumber(1));
  ESP.print(",");
  
  ESP.print(F("\"DO\": "));
  double do_value = sensorHub.getValueBySensorNumber(2);
  if (do_value >= 10000000000 || do_value <= -1000000000) do_value = -1.0;
  ESP.print(do_value);
  ESP.print(",");
  
  ESP.print(F("\"ORP\": "));                               
  ESP.print(sensorHub.getValueBySensorNumber(4)); 
  ESP.print(",");
  
  ESP.print(F("\"Conductivity\": "));
  double x = sensorHub.getValueBySensorNumber(3);
  ESP.print((9.0967*x + 38.275));
  ESP.print(",");

  ESP.print(F("\"lat\": "));
  ESP.print(GPS_lat, 6);
  ESP.print(",");
  ESP.print(F("\"lng\": "));
  ESP.print(GPS_lng, 6);
  
  ESP.println("}");

  // Debug
  Serial.print("{");
  
  Serial.print(F("\"pH\":  "));
  Serial.print(sensorHub.getValueBySensorNumber(0));
  Serial.print(",");
  
  Serial.print(F("\"Temperature\": "));
  Serial.print(sensorHub.getValueBySensorNumber(1));
  Serial.print(",");
  
  Serial.print(F("\"DO\": "));
  Serial.print(do_value);
  Serial.print(",");
  
  Serial.print(F("\"ORP\": "));                               
  Serial.print(sensorHub.getValueBySensorNumber(4)); 
  Serial.print(",");
  
  Serial.print(F("\"Conductivity\": "));
  Serial.print((9.0967*x + 38.275));
  Serial.print(",");

  Serial.print(F("\"lat\": "));
  Serial.print(GPS_lat, 6);
  Serial.print(",");
  Serial.print(F("\"lng\": "));
  Serial.print(GPS_lng, 6);
  
  Serial.println("}");
}
