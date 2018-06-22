#include <TinyGPS++.h>

#define GPS Serial2
TinyGPSPlus gpsParser;
char GPSstring[100];

void setup()
{
  Serial.begin(115200);
  GPS.begin(9600);
}

void loop()
{
  if (GPS.available()) {
      char c = GPS.read();
      if (gpsParser.encode(c)) displayInfo();
  }
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gpsParser.location.isValid())
  {
    Serial.print(gpsParser.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gpsParser.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Altitude: "));
  if (gpsParser.altitude.isValid())
  {
    Serial.print(gpsParser.altitude.meters());
    Serial.print(F("M"));
  }
  else
  {
    Serial.print(F("INVALID"));
  }


  Serial.print(F("  Date/Time: "));
  if (gpsParser.date.isValid())
  {
    Serial.print(gpsParser.date.month());
    Serial.print(F("/"));
    Serial.print(gpsParser.date.day());
    Serial.print(F("/"));
    Serial.print(gpsParser.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  
  Serial.print(F(" "));
  if (gpsParser.time.isValid())
  {
    if (gpsParser.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gpsParser.time.hour());
    Serial.print(F(":"));
    if (gpsParser.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gpsParser.time.minute());
    Serial.print(F(":"));
    if (gpsParser.time.second() < 10) Serial.print(F("0"));
    Serial.print(gpsParser.time.second());
    Serial.print(F("."));
    if (gpsParser.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gpsParser.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
