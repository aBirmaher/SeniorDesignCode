//Code Written by: Alan Birmaher
//UCF Senior Design 2 
//Group 6

//Include Libraries
#include <Wire.h>

//Setup constants for relays
#define RELAY_ON 0
#define RELAY_OFF 1
#define Relay_1  2
#define Relay_2  3
#define Relay_3  4
#define Relay_4  5
#define changeTime 6
#define Emergency 7
#define changeCurrent 8

//Global variables
int flag = 3;                 //Create flag
int buttonState = 0;          //Button State Flag
int sunriseHour = 6;          //Sunrise hour
int sunriseMin = 30;          //Sunrise minute
int sunsetHour = 20;          //Sunset hour
int sunsetMin = 30;           //Sunset minute

int nowHour;
int nowMin;

//Setup- Executed once
void setup() {

  //Initialize the IC2 bus
  Serial.begin(9600);

  //Current time set
  changeNow();

  //Power on at
  Serial.print("Arduino on at ");
  if(nowHour < 10) {
    Serial.print("0");
  }
  Serial.print(nowHour);
  Serial.print(":");
  if(nowMin < 10) {
    Serial.print("0");
  }
  Serial.println(nowMin);

  //Sunrise time
  Serial.print("Sunrise time ");
  if(sunriseHour < 10) {
    Serial.print("0");
  }
  Serial.print(sunriseHour);
  Serial.print(":");
  if(sunriseMin < 10) {
    Serial.print("0");
  }
  Serial.println(sunriseMin);

  //Sunset time
  Serial.print("Sunset time ");
  if(sunsetHour < 10) {
    Serial.print("0");
  }
  Serial.print(sunsetHour);
  Serial.print(":");
  if(sunsetMin < 10) {
    Serial.print("0");
  }
  Serial.println(sunsetMin);
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

  //If past sunset time and lights are off, turn on
  if(nowHour > sunsetHour){
    if(flag != 1){
      
      //Call lights on function
      lightsOn();  
          
    } 
  }

  
  //If past sunset time and lights are off, turn on
  else if(nowHour == sunsetHour && nowMin >= sunsetMin){
    if(flag != 1){
    
      //Call lights on function
      lightsOn();  
          
    } 
  }

  //If past sunset time and lights are off, turn on
  else if(nowHour > sunriseHour){
    if(flag != 0){
      
      //Call lights off function
      lightsOff();
      
    }
  }

  //If past sunset time and lights are off, turn on
  else if(nowHour == sunriseHour && nowMin >= sunriseMin){
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

  //If changeNow button is pressed
  buttonState = digitalRead(changeCurrent);
  if (buttonState == HIGH){
    changeNow();
  }

}

//Turns on the lights and sets flags
void lightsOff(){

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
  if(nowHour < 10) {
    Serial.print("0");
  }
  Serial.print(nowHour);
  Serial.print(":");
  if(nowMin < 10) {
    Serial.print("0");
  }
  Serial.println(nowMin);
  
}

//Turns off lights and sets flags
void lightsOn(){
  
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
  if(nowHour < 10) {
    Serial.print("0");
  }
  Serial.print(nowHour);
  Serial.print(":");
  if(nowMin < 10) {    
    Serial.print("0");
  }
  Serial.println(nowMin);
  
}

//If change time mode button is pressed
void change(){

  //Current sunrise time printed
  Serial.print("Current sunrise time is: ");
  if(sunriseHour < 10) {    
    Serial.print("0");
  }
  Serial.print(sunriseHour);
  Serial.print(":");
  if(sunriseMin < 10) {    
    Serial.print("0");
  }
  Serial.println(sunriseMin);

  //Current sunset time printed
  Serial.print("Current sunset time is: ");
  if(sunsetHour < 10) {    
    Serial.print("0");
  }
  Serial.print(sunsetHour);
  Serial.print(":");
  if(sunsetMin < 10) {    
    Serial.print("0");
  }
  Serial.println(sunsetMin);

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
    if(temp1<0 &&temp1>59){

      Serial.println("Improper value entered, exiting change button function");
      return;
      
    }

    //If all values are correct set new value
    sunriseHour = temp1;
    sunriseMin = temp2;
    Serial.print("New sunrise time is: ");
    if(sunriseHour < 10) {    
     Serial.print("0");
    }
    Serial.print(sunriseHour);
    Serial.print(":");
    if(sunriseMin < 10) {    
      Serial.print("0");
    }
    Serial.println(sunriseMin);
    
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
    if(temp1<0 &&temp1>59){

      Serial.println("Improper value entered, exiting change button function");
      return;
      
    }

    //If all values are correct set new value
    sunsetHour = temp1;
    sunsetMin = temp2;
    Serial.print("New sunset time is: ");
    if(sunsetHour< 10) {    
      Serial.print("0");
    }
    Serial.print(sunsetHour);
    Serial.print(":");
    if(sunsetMin < 10) {    
      Serial.print("0");
    }
    Serial.println(sunsetMin);

  }

  //Changes flag
  flag = 3;
  
}

//If emergency button is pressed
void emergency(){
  
  //Record emergency on console
  Serial.print("EMERGENCY BUTTON PRESSED at ");  
  if(nowHour < 10) {
    Serial.print("0");
  }
  Serial.print(nowHour);
  Serial.print(":");
  if(nowMin < 10) {
    Serial.print("0");
  }
  Serial.println(nowMin);

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

void changeNow(){

  //Current sunrise time printed
  Serial.print("Current time is: ");
  if(nowHour < 10) {    
    Serial.print("0");
  }
  Serial.print(nowHour);
  Serial.print(":");
  if(nowMin < 10) {    
    Serial.print("0");
  }
  Serial.println(nowMin);

  // Ask for input and take in
  Serial.print("Enter new current hour (0-23): ");
  while(Serial.available() == 0) { }
  int temp1 = Serial.parseInt();
  Serial.println();

  //Check if input is wrong
  if(temp1<0 || temp1>23){

    Serial.println("Improper value entered, exiting change button function");
    return;
    
  }

  // Ask for input and take in
  Serial.print("Enter new current minute (0-59): ");
  while(Serial.available() == 0) { }
  int temp2 = Serial.parseInt();
  Serial.println();

  //Check if input is wrong
  if(temp2<0 || temp2>59){

    Serial.println("Improper value entered, exiting change button function");
    return;
    
  }

  //If all values are correct set new value
  nowHour = temp1;
  nowMin = temp2;
  Serial.print("New time is: ");
  if(nowHour < 10) {    
   Serial.print("0");
  }
  Serial.print(nowHour);
  Serial.print(":");
  if(nowMin < 10) {    
    Serial.print("0");
  }
  Serial.println(nowMin);

  //Changes flag
  flag = 3;
  
}
