//PRODUCT NAME
//Code Written by: Alan Birmaher
//UCF Senior Design 2 
//Group 6

//Include Libraries
#include <Wire.h>
#include "RTClib.h"
#include <UnoWiFiDevEd.h>

//Setup constants for relays
#define RELAY_ON 0
#define RELAY_OFF 1
#define Relay_1  2
#define Relay_2  3
#define Relay_3  4
#define Relay_4  5
#define changeTime 6
#define Emergency 7

//Declare an RTC_DS1307 object named rtc
RTC_DS1307 rtc;

//Global variables
int flag = 0;                 //Create flag
int buttonState = 0;          //Button State Flag
int sunriseHour = 6;          //Sunrise and Sunset times
int sunriseMin = 30;
int sunsetHour = 20;
int sunsetMin = 30;

//Setup- Executed once
void setup() {

  //Initialize the IC2 bus
  Serial.begin(9600);

  //Setup for wifi server
  Wifi.begin();
  Serial.println("Web Server is up"); 

  //Setup for real time clock
  //Initialize the real time clock device
  rtc.begin();

  //Check if rtc is running, if not set time from computer
  if (!rtc.isrunning()) {

    //If not running, set the time
    Wifi.println("RTC is not running");
    rtc.adjust(DateTime(__DATE__, __TIME__));

  }

  //Sets now variable to be time now
  DateTime now = rtc.now();

  //Power on at
  Serial.print("Arduino on at ");
  if(now.hour() < 10) {
    Wifi.print("0");
  }
  Serial.print(now.hour());
  Wifi.print(":");
  if(now.minute() < 10) {
    Wifi.print("0");
  }
  Wifi.println(now.minute());

  //Sunrise time
  Wifi.print("Sunrise time ");
  if(sunriseHour < 10) {
    Wifi.print("0");
  }
  Wifi.print(sunriseHour);
  Wifi.print(":");
  if(sunriseMin < 10) {
    Wifi.print("0");
  }
  Wifi.println(sunriseMin);

  //Sunset time
  Wifi.print("Sunset time ");
  if(sunsetHour < 10) {
    Wifi.print("0");
  }
  Wifi.print(sunsetHour);
  Wifi.print(":");
  if(sunsetMin < 10) {
    Wifi.print("0");
  }
  Wifi.println(sunsetMin);
  Wifi.println();

  //Initialize pins so all lights are off
  digitalWrite(Relay_1, RELAY_OFF);
  digitalWrite(Relay_2, RELAY_OFF);
  digitalWrite(Relay_3, RELAY_OFF);
  digitalWrite(Relay_4, RELAY_OFF);  

  //Setup all relays as outputs
  pinMode(Relay_1, OUTPUT);   
  pinMode(Relay_2, OUTPUT);  
  pinMode(Relay_3, OUTPUT);  
  pinMode(Relay_4, OUTPUT);    
  pinMode(Emergency, INPUT);   
  pinMode(changeTime, INPUT); 
  
}

// Executes repeatedly
void loop() {

  //Sets now variable to be time now
  DateTime now = rtc.now();

  //If past sunrise, turn on light
  if (now.hour() >= sunriseHour && now.hour() < sunsetHour && flag == 1) {
    if(now.minute() >= sunriseMin) {

      //Turn off relays for LED lights
      delay(1000); 
      digitalWrite(Relay_1, RELAY_OFF);
      delay(1000); 
      digitalWrite(Relay_2, RELAY_OFF);
      delay(1000); 
      digitalWrite(Relay_3, RELAY_OFF);
      delay(1000); 
      digitalWrite(Relay_4, RELAY_OFF);
      delay(1000); 
    
      //Flag for light off
      flag = 0;

      //Note of time light switches
      Wifi.print("Lights off at ");
      if(now.hour() < 10) {
        Wifi.print("0");
      }
      Wifi.print(now.hour());
      Wifi.print(":");
      if(now.minute() < 10) {
        Wifi.print("0");
      }
      Wifi.println(now.minute());
      
    }
  }

  //Else if check if past sunset, turn off light
  else if (now.hour() >= sunsetHour && now.hour() > sunriseHour && flag == 0) {
    if(now.minute() >= sunsetMin) {

      //Turn on relays for LED lights
      delay(1000); 
      digitalWrite(Relay_1, RELAY_ON);
      delay(1000); 
      digitalWrite(Relay_2, RELAY_ON);
      delay(1000); 
      digitalWrite(Relay_3, RELAY_ON);
      delay(1000); 
      digitalWrite(Relay_4, RELAY_ON);
      delay(1000); 
  
      //Flag for light on
      flag = 1;

      //Note of time light switches
      Wifi.print("Lights on at ");
      if(now.hour() < 10) {
        Wifi.print("0");
      }
      Wifi.print(now.hour());
      Wifi.print(":");
      if(now.minute() < 10) {    
        Wifi.print("0");
      }
      Wifi.println(now.minute());
      
    }
  }

  //If make change is pressed
  buttonState = digitalRead(changeTime);
  if (buttonState == HIGH)
    change();

  //If emergency button is pressed
  buttonState = digitalRead(Emergency);
  if (buttonState == HIGH)
    emergency();

}

void change(){

  //PUT CODE
  
}

//If emergency button is pressed
void emergency(){

  //Sets now variable to be time now
  DateTime now = rtc.now();

  
  //Record emergency on console
  Wifi.print("EMERGENCY BUTTON PRESSED at ");  
  if(now.hour() < 10) {
    Wifi.print("0");
  }
  Wifi.print(now.hour());
  Wifi.print(":");
  if(now.minute() < 10) {
    Wifi.print("0");
  }
  Wifi.println(now.minute());

  //Sets now variable to be time now
  for(int i = 0; i<5; i++){
    for(int j = 0; j<30; j++){
      
       //Turn on relays for LED lights
      digitalWrite(Relay_1, RELAY_ON);
      digitalWrite(Relay_3, RELAY_ON);
      digitalWrite(Relay_2, RELAY_OFF); 
      digitalWrite(Relay_4, RELAY_OFF);
      delay(1000); 
      digitalWrite(Relay_1, RELAY_OFF);
      digitalWrite(Relay_3, RELAY_OFF);
      digitalWrite(Relay_2, RELAY_ON); 
      digitalWrite(Relay_4, RELAY_ON);
      delay(1000); 
        
    }
    
  }

  //Turn lights back on after completion 
  digitalWrite(Relay_1, RELAY_ON);
  digitalWrite(Relay_3, RELAY_ON);
  digitalWrite(Relay_2, RELAY_ON); 
  digitalWrite(Relay_4, RELAY_ON);
  flag= 1;

}
