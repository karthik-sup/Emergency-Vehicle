//Include Libraries
#include <SPI.h>
#include <RF24.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>


LiquidCrystal lcd(14, 15, 16, 17, 18, 19);
int Contrast = 0;
int RXPin = 6;  //declaring recieving pin of GPS
int TXPin = 7;  //declaring transmiting pin of GPS


//create an RF24 object
RF24 radio(9, 8);  // CE, CSN


//address through which two modules communicate.
const byte address[6] = "00001";


TinyGPSPlus gps;
SoftwareSerial SerialGPS(RXPin, TXPin);


double lngi = -86.167524; //example coordinates for size
double latr,lngr;
double late,lnge;
double distance;
float k=0;


void setup()
{
  Serial.begin(38400);
  SerialGPS.begin(38400);
  lcd.begin(16, 2);
  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);
  
  //Set module as receiver
  radio.startListening();
}
void loop()
{
  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
      {showData();
      checkemer(); 
      getDistance();
      }
      
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("GPS NOT DETECTED!");
    while(true); 
  }
}
void showData()
{
  if (gps.location.isValid())
  {
    latr=gps.location.lat();
    lngr=gps.location.lng();
  }
  else
  {
    Serial.println("Location is not available");
  } 
}


void checkemer()
{
  if (radio.available())
  {
    radio.read(&late, sizeof(lngi));
    Serial.println(late);
    radio.read(&lnge, sizeof(lngi));
    Serial.println(lnge);
  }
  
}
void getDistance() {
    // haversine formula for distance between two points on sphere
    float dlat = radians(late - latr);
    float dlon = radians(lnge - lngr);
    float a = sq(sin(dlat/2)) + cos(latr) * cos(late) * sq(sin(dlon/2));
    float c = 2 * atan2(sqrt(a), sqrt(1-a));
    int R = 6371; // mean radius of Earth in KM
    distance = R * c*1000;
    Serial.println("distance");
    Serial.println(distance);
      if (k>distance)
      {
        Serial.println("Emergency Vehicle approaching");
        lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print("Distance:");
  lcd.setCursor(10,0);
  lcd.print(distance);
  lcd.setCursor(0,1); 
  lcd.print("Emergency Vehicle approaching");
  delay(1000);
    }
        Serial.println("Safe Journey");
        lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print("Safe Journey");
    k=distance;
  delay(1000);
}
