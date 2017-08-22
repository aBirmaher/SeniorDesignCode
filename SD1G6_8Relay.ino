//PRODUCT NAME
//Code Written by: Alan Birmaher
//UCF Senior Design
//Group 6

//Include Libraries
#include <Wire.h>
#include <UnoWiFiDevEd.h>
#include "RTClib.h"

//Setup constants for relays
#define RELAY_ON 0
#define RELAY_OFF 1
#define Relay_1  2
#define Relay_2  3
#define Relay_3  4
#define Relay_4  5
#define Relay_5  6  
#define Relay_6  7
#define Relay_7  8
#define Relay_8  9

//Declare an RTC_DS1307 object named rtc
RTC_DS1307 rtc;

//Global variables
int flag = 0;                 //Create flag
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
  Wifi.println("Web Server is up");       //Wifi Console
  //End of setup got wifi server

  //Setup for real time clock
  //Initialize the real time clock device
  rtc.begin();

  //Check if rtc is running, if not set time from computer
  if (!rtc.isrunning()) {

    //If not running, set the time
    Wifi.println("RTC is not running");
    rtc.adjust(DateTime(__DATE__, __TIME__));
    
  }
  //End of setup for real time clock

  //Sets now variable to be time now
  DateTime now = rtc.now();

  //Power on at
  Wifi.print("Arduino on at ");
  if(now.hour() < 10) {
    Wifi.print("0");
  }
  Wifi.print(now.hour());
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

  //Setup for relays for LEDs
  //Initialize pins so all lights are off
  digitalWrite(Relay_1, RELAY_OFF);
  digitalWrite(Relay_2, RELAY_OFF);
  digitalWrite(Relay_3, RELAY_OFF);
  digitalWrite(Relay_4, RELAY_OFF);  
  digitalWrite(Relay_5, RELAY_OFF);
  digitalWrite(Relay_6, RELAY_OFF);
  digitalWrite(Relay_7, RELAY_OFF);
  digitalWrite(Relay_8, RELAY_OFF); 

  //Setup all relays as outputs
  pinMode(Relay_1, OUTPUT);   
  pinMode(Relay_2, OUTPUT);  
  pinMode(Relay_3, OUTPUT);  
  pinMode(Relay_4, OUTPUT);    
  pinMode(Relay_5, OUTPUT);   
  pinMode(Relay_6, OUTPUT);  
  pinMode(Relay_7, OUTPUT);  
  pinMode(Relay_8, OUTPUT);
  //End of setup of relays for LEDs
  
}

// Executes repeatedly
void loop() {

  //Keep wifi alive
  while(Wifi.available()){
      process(Wifi);
   }

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
      digitalWrite(Relay_5, RELAY_OFF);
      delay(1000); 
      digitalWrite(Relay_6, RELAY_OFF);
      delay(1000); 
      digitalWrite(Relay_7, RELAY_OFF);
      delay(1000); 
      digitalWrite(Relay_8, RELAY_OFF);
    
      //Flag for light off
      flag = 0;

      //Note of time light switches (Wifi console)
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
      digitalWrite(Relay_5, RELAY_ON);
      delay(1000); 
      digitalWrite(Relay_6, RELAY_ON);
      delay(1000); 
      digitalWrite(Relay_7, RELAY_ON);
      delay(1000); 
      digitalWrite(Relay_8, RELAY_ON);
      delay(1000); 
  
      //Flag for light on
      flag = 1;

      //Note of time light switches (Wifi console)
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
  
  //Wait 1 min before checking again
  delay(60000);

}

void process(WifiData client) {
  // read the command
  String command = client.readStringUntil('/');

  if (command == "set") {
    WebServer(client);
  }

  else if (command == "sunrise") {
    sunriseSet(client);
  }

  else if (command == "sunset") {
    sunsetSet(client);
  }
}

void WebServer(WifiData client) {

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.print("<html>");
  client.print("<head> <title>Setup Page</title> </head>");
  client.print("<body>");
  client.print("<h2>Setup Page</h2><br>");
  client.print("In the address bar above, update sunrise or sunset times as follows:<br>");
  client.print("Replace the word \"set/\" with sunrise or sunset depending which you want <br>");
  client.print("to change followed by a \"/\" then the hour followed by another \"/\" and the  ");
  client.print("minute. For exaple, for setting sunrise to 6:35PM you would replace \"/get\" above");
  client.print("with \"/sunset/18/35. When you are done entering the value press enter.\"");
  client.print("</body>");
  client.print("</html>");
  client.print(DELIMITER); // very important to end the communication !!!

}

void sunriseSet(WifiData client) {

  //Throwaway variables
  char temp;

  //Set new sunrise time
  sunriseHour = client.parseInt();
  temp = client.read();
  sunriseMin = client.parseInt();

  // Send feedback to client
  client.print("Current Sunrise time: ");
  client.print(sunriseHour);
  client.print(":");
  client.println(sunriseMin);
  client.print("Current Sunset time: ");
  client.print(sunsetHour);
  client.print(":");
  client.println(sunsetMin);
  client.print(EOL);    //char terminator
  
}

void sunsetSet(WifiData client) {

  //Throwaway variables
  char temp;

  //Set new sunset time
  sunsetHour = client.parseInt();
  temp = client.read();
  sunsetMin = client.parseInt();

  // Send feedback to client
  client.print("Current Sunrise time: ");
  client.print(sunriseHour);
  client.print(":");
  client.println(sunriseMin);
  client.print("Current Sunset time: ");
  client.print(sunsetHour);
  client.print(":");
  client.println(sunsetMin);
  client.print(EOL);    //char terminator

}

void emergencyLights(){

  //Time now
  //Sets now variable to be time now
  DateTime now = rtc.now();
  
  
  //Flash lights for 5 minutes
  while (){

    //Turn on relays for LED lights
    delay(100); 
    digitalWrite(Relay_1, RELAY_ON);
    delay(100); 
    digitalWrite(Relay_2, RELAY_ON);
    delay(100); 
    digitalWrite(Relay_3, RELAY_ON);
    delay(100); 
    digitalWrite(Relay_4, RELAY_ON);
    delay(100); 

    
  }
  
}

