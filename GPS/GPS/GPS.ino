#include <TinyGPS++.h>

#define GPS Serial1
TinyGPSPlus gpsParser;
char GPSstring[100];

// A sample NMEA stream.
const char *gpsParserStream =
  "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
  "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
  "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
  "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
  "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
  "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";

void setup()
{
  Serial.begin(115200);
  GPS.begin(9600);
}

void loop()
{
  if (GPS.available()) {
//    String s = GPS.readStringUntil('\n');
//    s = s + "\n";
//    s.toCharArray(GPSstring, sizeof(GPSstring));
//
//    gpsParser.encode(&GPSstring);
//    displayInfo();
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
