#include "Arduino.h"
#include "SoftwareSerial.h"
#include <TinyGPS++.h>

int RXPin = 16;
int TXPin = 17;

int GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()
{
    // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);
  // display.clearDisplay();

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
  Serial.begin(115200);
  }
  
  void loop(){
        while (gpsSerial.available() > 0){
    if (gps.encode(gpsSerial.read())){
         displayInfo();
        }
    }


  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      Serial.println("No GPS detected");
      while(true);
    }
    
  }

void displayInfo(){
  Serial.print(F("Location: "));
  if (gps.location.isValid()){
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
    }  
  else{
    Serial.println("Location: Not Available");
    }
}
