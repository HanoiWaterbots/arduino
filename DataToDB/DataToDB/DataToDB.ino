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

/* Main */

void setup() {
  Serial.begin(115200);
  rtc.setup();
  sensorHub.setup();
  sdService.setup();
  GPS.begin(9600);
}

void loop() {
	rtc.update();
	sensorHub.update();
	sdService.update();

  if (GPS.available()) {
      char c = GPS.read();
      if (gpsParser.encode(c)) parseGPS();
  }

	// ************************* Serial debugging ******************
	if(millis() - updateTime > 2000)
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
  Serial.print("{");
 
  Serial.print(F("\"Timestamp\": "));
  Serial.print(rtc.day);
  Serial.print("/");
  Serial.print(rtc.month);
  Serial.print("/");
  Serial.print(rtc.year);
  Serial.print("    ");
  Serial.print(rtc.hour);
  Serial.print(":");
  Serial.print(rtc.minute);
  Serial.print(":");
  Serial.print(rtc.second);
  Serial.print(",");
  
  Serial.print(F("\"pH\":  "));
  Serial.print(sensorHub.getValueBySensorNumber(0));
  Serial.print(",");
  
  Serial.print(F("\"Temperature\": "));
  Serial.print(sensorHub.getValueBySensorNumber(1));
  Serial.print(",");
  
  Serial.print(F("\"DO\": "));
  Serial.print(sensorHub.getValueBySensorNumber(2));
  Serial.print(",");
  
  Serial.print(F("\"ORP\": "));                               
  Serial.print(sensorHub.getValueBySensorNumber(4)); 
  Serial.print(",");
  
  Serial.print(F("\"Conductivity\": "));
  double x = sensorHub.getValueBySensorNumber(3);
  Serial.print((9.0967*x + 38.275));
  Serial.print(",");

  Serial.print(F("\"lat\": "));
  Serial.print(GPS_lat, 6);
  Serial.print(",");
  Serial.print(F("\"lng\": "));
  Serial.print(GPS_lng, 6);
  
  Serial.println("}");
}
