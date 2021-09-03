
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
bool flag = true;
//LED//////////////////////////////
#define LED_BUILTIN 2
////////coordinates///////////////
#define latitude1 31.8014314
#define longitude1 34.6546769
/////////////////////////////////
float lati;
float longi;

//GPS section///////////////////////
static const int RXPin = 16, TXPin = 17;
static const uint32_t GPSBaud = 9600;


// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

//Servo section/////////////////////
static const int servoPin = 13;  // defines pin number for PWM
Servo servo1;  // Create object for servo motor



//LCD///////////////////////////////////////////////////
// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 
/////////////////////////////////////////////////////////

//int flag = 0;//initial constant to 0


void setup()
{
  Serial.begin(115200);
  Serial.begin(9600);
  ss.begin(GPSBaud);
  servo1.attach(servoPin);  // start the library

  /////////////LCD/////////////
   // initialize LCD
  lcd.clear();
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  //clear LCD last data
  lcd.clear(); 
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("GPS SEARCH...");
  delay(3000);
  
  pinMode(LED_BUILTIN,OUTPUT);

}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

//function section
void displayInfo()
{
  ///////////////////////////////////////////////
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    
    if (flag)//entering only if flag is true
    {
      //clear LCD last data
      lcd.clear(); 
      // set cursor to first column, first row
      lcd.setCursor(0, 0);
      // print message
      lcd.print("GPS 3D FIX");
    
    
    Serial.print(gps.location.lat(), 7);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 7);
    
    if (gps.location.lat()>latitude1-0.0000500 && gps.location.lat()<latitude1+0.0000500 && gps.location.lng()>longitude1-0.0000500 && gps.location.lng()<longitude1+0.0000500)
    {
      
      flag = false;//change flag from true to false
      lati=gps.location.lat();
      longi=gps.location.lng();
      
//  //Motor operation/////////////////
//  
//  //for section servo motor/////////////////////////////////////////////////////////////////////////////////
  for(int posDegrees = 0; posDegrees <= 180; posDegrees++)
  {
  servo1.write(posDegrees);
  Serial.println(posDegrees);
  delay(20);
  }

  for(int posDegrees = 180; posDegrees >= 0; posDegrees--) 
  {
  servo1.write(posDegrees);
  Serial.println(posDegrees);
  delay(20);
  }
  ///////////////////////////////////////////////////////////////

  ///LCD Display//////////////////////////////////
//
 
//  //clear LCD last data
  lcd.clear(); 
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Lati:"); lcd.print(lati,7);
  // set cursor to first column, second row
  lcd.setCursor(0,1);
  lcd.print("long:"); lcd.print(longi,7);
  
    }
   
  }
  
  }
  
  
  else
  {
    Serial.print(F("NO GPS SIGNAL!"));
    
//  ///LCD Display//////////////////////////////////
  //clear LCD last data
  lcd.clear(); 
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("NO GPS!");
  }
  ///////////////////////////////////////////////
  
  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("UNKNOW"));
  }
  ///////////////////////////////////////////////
  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour()+3);
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("UNKNOW"));
  }
  //////////////////////////////////////////////////
  Serial.println();
}
