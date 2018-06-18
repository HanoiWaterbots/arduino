#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "GravitySensorHub.h"
#include "GravityRtc.h"
#include "OneWire.h"
#include "SdService.h"
#include "Debug.h"
#include <SoftwareSerial.h>
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
SoftwareSerial ESP(10, 11); // RX, TX

/* Main */

void setup() {
  Serial.begin(57600);
  rtc.setup();
  sensorHub.setup();
  sdService.setup();
  GPS.begin(9600);
  ESP.begin(115200);
}

void loop() {
	rtc.update();
	sensorHub.update();
	sdService.update();

  if (GPS.available()) {
      char c = GPS.read();
      if (gpsParser.encode(c)) parseGPS();
  }

	// ************************* ESP debugging ******************
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
  ESP.print("{");
 
  ESP.print(F("\"timestamp\": \""));
  ESP.print(rtc.year);
  ESP.print("-");
  ESP.print(rtc.month);
  ESP.print("-");
  ESP.print(rtc.day);
  ESP.print("T");
  ESP.print(rtc.hour);
  ESP.print(":");
  ESP.print(rtc.minute);
  ESP.print(":");
  ESP.print(rtc.second);
  ESP.print("\",");
  
  ESP.print(F("\"pH\":  "));
  ESP.print(sensorHub.getValueBySensorNumber(0));
  ESP.print(",");
  
  ESP.print(F("\"Temperature\": "));
  ESP.print(sensorHub.getValueBySensorNumber(1));
  ESP.print(",");
  
  ESP.print(F("\"DO\": "));
  ESP.print(sensorHub.getValueBySensorNumber(2));
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
 
  Serial.print(F("\"timestamp\": \""));
  Serial.print(rtc.year);
  Serial.print("-");
  Serial.print(rtc.month);
  Serial.print("-");
  Serial.print(rtc.day);
  Serial.print("T");
  Serial.print(rtc.hour);
  Serial.print(":");
  Serial.print(rtc.minute);
  Serial.print(":");
  Serial.print(rtc.second);
  Serial.print("\",");
  
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
  Serial.print((9.0967*x + 38.275));
  Serial.print(",");

  Serial.print(F("\"lat\": "));
  Serial.print(GPS_lat, 6);
  Serial.print(",");
  Serial.print(F("\"lng\": "));
  Serial.print(GPS_lng, 6);
  
  Serial.println("}");
}
