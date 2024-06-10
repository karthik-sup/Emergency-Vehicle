//Include Libraries
#include <SPI.h>
#include <RF24.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
int RXPin = 6;
int TXPin = 7;
//create an RF24 object
RF24 radio(9, 8);  // CE, CSN
//address through which two modules communicate.
const byte address[6] = "00001";


TinyGPSPlus gps;
SoftwareSerial SerialGPS(RXPin, TXPin);


double lati = 18.008087;  //example coordinates
double lngi = 79.554321;
double latr, lngr;


void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600);


  radio.begin(); //set the address
  radio.openWritingPipe(address);  //Set module as transmitter
  radio.stopListening();
}
void loop()
{
while (SerialGPS.available() > 0) 
{
        if (gps.encode(SerialGPS.read())) 
  {
            if (gps.location.isValid()) 
      {
                latr = gps.location.lat();
                Serial.println("late");
                Serial.println(latr,6);
                radio.write(&latr, sizeof(lati));
                lngr = gps.location.lng();
                Serial.println("lnge");
                Serial.println(lngr,6);
                radio.write(&lngr, sizeof(lngi));
            }
           delay(5000);
        }
    }
}
