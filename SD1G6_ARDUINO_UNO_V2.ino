r//Code Written by: Alan Birmaher
//UCF Senior Design 2 
//Group 6

//Include Libraries
#include <Wire.h>
#include <EEPROM.h>
#include "RTClib.h"

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
int flag = 3;                 //Create flag
int buttonState = 0;          //Button State Flag
int addrSunriseHour= 0;       //Address of sunrise hour in EEPROM
int addrSunriseMin= 1;        //Address of sunrise min in EEPROM
int addrSunsetHour= 2;        //Address of sunset hour in EEPROM
int addrSunsetMin= 3;         //Address of sunset min in EEPROM                                            

//Setup- Executed once
void setup() {

  //Initialize the IC2 bus
  Serial.begin(9600);
  
  //Setup for real time clock
  //Initialize the real time clock device
  rtc.begin();

  //Check if rtc is running, if not set time from computer
  if (!rtc.isrunning()) {

    //If not running, set the time
    Serial.println("RTC is not running");
    rtc.adjust(DateTime(__DATE__, __TIME__));

  }

  //Sets now variable to be time now
  DateTime now = rtc.now();

  //Power on at
  Serial.print("Arduino on at ");
  if(now.hour() < 10) {
    Serial.print("0");
  }
  Serial.print(now.hour());
  Serial.print(":");
  if(now.minute() < 10) {
    Serial.print("0");
  }
  Serial.println(now.minute());

  //Sunrise time
  Serial.print("Sunrise time ");
  if(EEPROM.read(addrSunriseHour) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(addrSunriseHour));
  Serial.print(":");
  if(EEPROM.read(addrSunriseMin) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(addrSunriseMin));

  //Sunset time
  Serial.print("Sunset time ");
  if(EEPROM.read(addrSunsetHour) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(addrSunsetHour));
  Serial.print(":");
  if(EEPROM.read(addrSunsetMin) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(addrSunsetMin));
  Serial.println();

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
  pinMode(changeTime, INPUT); 
  pinMode(Emergency, INPUT);   
  
}

// Executes repeatedly
void loop() {

  //Sets now variable to be time now
  DateTime now = rtc.now();

  //If past sunset time and lights are off, turn on
  if(now.hour() > EEPROM.read(addrSunsetHour)){
    if(flag != 1){
      
      //Call lights on function
      lightsOn();  
          
    } 
  }

  
  //If past sunset time and lights are off, turn on
  else if(now.hour() == EEPROM.read(addrSunsetHour) && now.minute() >= EEPROM.read(addrSunsetMin)){
    if(flag != 1){
      
      //Call lights on function
      lightsOn();  
          
    } 
  }

  //If past sunset time and lights are off, turn on
  else if(now.hour() > EEPROM.read(addrSunriseHour)){
    if(flag != 0){
      
      //Call lights off function
      lightsOff();
      
    }
  }

  //If past sunset time and lights are off, turn on
  else if(now.hour() == EEPROM.read(addrSunriseHour) && now.minute() >= EEPROM.read(addrSunriseMin)){
    if(flag != 0){
      
      //Call lights off function
      lightsOff();
      
    }
  }

  //If it is earlier than sunrise, lights should be on, turn on lights if off
  else{
    if(flag != 1){

      //Calls lights on function
      lightsOn();
      
    } 
  }
  
  //If make change is pressed
  buttonState = digitalRead(changeTime);
  if (buttonState == HIGH){
    change();
  }

  //If emergency button is pressed
  buttonState = digitalRead(Emergency);
  if (buttonState == HIGH){
    emergency();
  }

}

//Turns on the lights and sets flags
void lightsOff(){

  //Sets now variable to be time now
  DateTime now = rtc.now();

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
  Serial.print("Lights off at ");
  if(now.hour() < 10) {
    Serial.print("0");
  }
  Serial.print(now.hour());
  Serial.print(":");
  if(now.minute() < 10) {
    Serial.print("0");
  }
  Serial.println(now.minute());
  
}

//Turns off lights and sets flags
void lightsOn(){

  //Sets now variable to be time now
  DateTime now = rtc.now();

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
  Serial.print("Lights on at ");
  if(now.hour() < 10) {
    Serial.print("0");
  }
  Serial.print(now.hour());
  Serial.print(":");
  if(now.minute() < 10) {    
    Serial.print("0");
  }
  Serial.println(now.minute());
  
}

//If change time mode button is pressed
void change(){

  //Set timeout value

  //Current sunrise time printed
  Serial.print("Current sunrise time is: ");
  if(EEPROM.read(addrSunriseHour) < 10) {    
    Serial.print("0");
  }
  Serial.print(EEPROM.read(addrSunriseHour));
  Serial.print(":");
  if(EEPROM.read(addrSunriseMin) < 10) {    
    Serial.print("0");
  }
  Serial.println(EEPROM.read(addrSunriseMin));

  //Current sunset time printed
  Serial.print("Current sunset time is: ");
  if(EEPROM.read(addrSunsetHour) < 10) {    
    Serial.print("0");
  }
  Serial.print(EEPROM.read(addrSunsetHour));
  Serial.print(":");
  if(EEPROM.read(addrSunsetMin) < 10) {    
    Serial.print("0");
  }
  Serial.println(EEPROM.read(addrSunsetMin));

  //Print instructions for user
  Serial.print("Please enter 1 to change sunrise time or 2 to change sunset time: ");
  
  //Read value from console
  while(Serial.available() == 0) { }
  int tempFlag = Serial.parseInt();
  Serial.println();

  //if option is not 1 or 2 try again
  if(tempFlag != 1 && tempFlag != 2){

    Serial.println("Incorrect value, exiting back to loop funciton");
    return;
    
  }

  //Sunrise
  else if(tempFlag == 1){

    // Ask for input and take in
    Serial.print("Enter new sunrise hour (0-23): ");
    while(Serial.available() == 0) { }
    int temp1 = Serial.parseInt();
    Serial.println();

    //Check if input is wrong
    if(temp1<0 &&temp1>23){

      Serial.println("Improper value entered, exiting change button function");
      return;
      
    }

    // Ask for input and take in
    Serial.print("Enter new sunrise minute (0-59): ");
    while(Serial.available() == 0) { }
    int temp2 = Serial.parseInt();
    Serial.println();

    //Check if input is wrong
    if(temp2<0 &&temp2>59){

      Serial.println("Improper value entered, exiting change button function");
      return;
      
    }

    //If all values are correct set new value
    EEPROM.write(addrSunriseHour, temp1);
    EEPROM.write(addrSunriseMin, temp2);
    Serial.print("New sunrise time is: ");
    if(EEPROM.read(addrSunriseHour) < 10) {    
     Serial.print("0");
    }
    Serial.print(EEPROM.read(addrSunriseHour));
    Serial.print(":");
    if(EEPROM.read(addrSunriseMin) < 10) {    
      Serial.print("0");
    }
    Serial.println(EEPROM.read(addrSunriseMin));
    
  }

  else if(tempFlag == 2){

    // Ask for input and take in
    Serial.print("Enter new sunset hour (0-23): ");
    while(Serial.available() == 0) { }
    int temp1 = Serial.parseInt();
    Serial.println();

    //Check if input is wrong
    if(temp1<0 &&temp1>23){

      Serial.println("Improper value entered, exiting change button function");
      return;
      
    }
    
    // Ask for input and take in
    Serial.print("Enter new sunset minute (0-59): ");
    while(Serial.available() == 0) { }
    int temp2 = Serial.parseInt();
    Serial.println();

    //Check if input is wrong
    if(temp2<0 &&temp2>59){

      Serial.println("Improper value entered, exiting change button function");
      return;
      
    }

    //If all values are correct set new value
    EEPROM.write(addrSunsetHour, temp1);
    EEPROM.write(addrSunsetMin, temp2);
    Serial.print("New sunset time is: ");
    if(EEPROM.read(addrSunsetHour)< 10) {    
      Serial.print("0");
    }
    Serial.print(EEPROM.read(addrSunsetHour));
    Serial.print(":");
    if(EEPROM.read(addrSunsetMin) < 10) {    
      Serial.print("0");
    }
    Serial.println(EEPROM.read(addrSunsetMin));

  }

  //Changes flag
  flag = 3;
  
}

//If emergency button is pressed
void emergency(){

  //Sets now variable to be time now
  DateTime now = rtc.now();

  
  //Record emergency on console
  Serial.print("EMERGENCY BUTTON PRESSED at ");  
  if(now.hour() < 10) {
    Serial.print("0");
  }
  Serial.print(now.hour());
  Serial.print(":");
  if(now.minute() < 10) {
    Serial.print("0");
  }
  Serial.println(now.minute());

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

