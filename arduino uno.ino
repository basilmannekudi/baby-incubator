#include "MAX30100_PulseOximeter.h"
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SoftwareSerial.h>
 // RX, TX
const int SW_L = A0;
const int SW_OK = A1;
const int SW_R = A2;
const int fanPin = 3;
const int heaterPin = 4;
const int tempset;
const int bpset;
const int bpThreshold;
const int sp02set;
const int sp02Threshold;
const int temThreshold;
const int humidset;
const int humidThreshold;
bool fanState = false;
bool heatState = false;
  float bp;
  float spo2;
  float humidity;
  float temp; 
SoftwareSerial mySerial(10, 11);
#include <Wire.h>
PulseOximeter pox;
    // including the library of DHT11 temperature and humidity 
 #define DHTPIN 2      // Selecting the pin at which we have connected DHT11
 #define DHTTYPE DHT11 // Selecting the type of DHT sensors
 LiquidCrystal_I2C lcd(0x27, 16, 2);
 DHT dht ( DHTPIN, DHTTYPE ) ;
 void setup ( ) {
  
  pinMode(SW_L,INPUT_PULLUP);
  pinMode(SW_R,INPUT_PULLUP);
  pinMode(SW_OK,INPUT_PULLUP);
  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
   if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
   //    pox.setOnBeatDetectedCallback(onBeatDetected);
   dht.begin () ;    // The sensor will start working
// Print a message to the LCD.
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0, 0);  
lcd.print("Baby Incubator");
delay(1000);
lcd.clear();
 }
 void loop ( ) {
   // Reading temperature or humidity may take about 2 seconds because it is a very slow sensor.
   pox.update();
   bp = pox.getHeartRate();
   spo2 = pox.getSpO2();
   humidity = dht.readHumidity () ; 
   temp = dht.readTemperature () ;  
    mySerial.println("b=");
    mySerial.print(bp);
    mySerial.println("s=");
    mySerial.print(spo2);
    mySerial.println("h=");
    mySerial.print(humidity);
    mySerial.println("t=");
    mySerial.print(temp);
   displayVal();
   compare();
 }
 void compare()
 {
  if(temp<tempset)
  {
     digitalWrite(4,HIGH);//Lamp
     digitalWrite(3,LOW);//Lamp
     fanState = false;
     heatState = true;
    }
    else if(tempset>(tempset+temThreshold))
    {
      digitalWrite(3,HIGH);//fan
      digitalWrite(4,LOW);//Lamp
     fanState = true;
     heatState = false;
      }
      if(bp<bpset)
      {
        // alret
        }
        else if(bp>bpset+bpThreshold)
        {
          //alert
          }
  }
 void displayVal()
 {
    static uint32_t oldtime=millis();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.setCursor(2,0);
    lcd.print(temp);
    lcd.setCursor(9,0);
    lcd.print("H:");
    lcd.setCursor(11,0);
    lcd.print ( humidity ) ;
    
    if ( (millis()-oldtime) > 500) {
       oldtime = millis();

    lcd.setCursor(0, 0);
    lcd.print("BP:");
    lcd.setCursor(2,0);
    lcd.print(bp);
    lcd.setCursor(9,0);
    lcd.print("SpO2:");
    lcd.setCursor(11,0);
    lcd.print (spo2) ;
  
    }

  }
