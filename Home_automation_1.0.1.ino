/////////////////////////////////////////////////////////////////
//                                                             //
// Author: Mahmoud Youssof Mahmoud <mahmoud.yusof27@gmail.com> //
// Organiztion: Catalysis => Ain Shams University              //
// Project: Stand alone smart home.                            //
// Copy rights 2018; All rights reserved.                      //
//                                                             //
/////////////////////////////////////////////////////////////////

// includes
#include <Servo.h>

// defining all varibles for later change
// you should change the password for your personal usage
Servo door; // defining a servo object ***

// making all needed variables (pins, values, states)
int temp_pin=A1;
int fan=8;
int living_pin = 5;
int bed_pin = 4;
int doorpin=9;
int alarm_pin = 11;
int pir_pin = 7;
int tx=1;
int rx=0;
// -------------------------------------------------------------------------------------------------------------------------
int temp;
// -------------------------------------------------------------------------------------------------------------------------
int err_count = 0;
char input[30];
String str_in;
String password = "1234";            // put your password of choice here :)
int delay_time = 1;      // 3 minutes
int mode = 1;
int door_state = 0;
double temp_timer = 0;
double pir_timer = 0;
// last time you have checked .. time..... last check time >> lc_time


// setup.... duh.
void setup() {
  // starting the serial communication at a baud rate of 9600
  Serial.begin(9600);
  pinMode(fan, OUTPUT);
  pinMode(temp_pin, INPUT);
  pinMode(doorpin, OUTPUT);
  pinMode(living_pin, OUTPUT);
  pinMode(bed_pin, OUTPUT);
  pinMode(alarm_pin, OUTPUT);
  pinMode(pir_pin, INPUT);
  pinMode(tx, OUTPUT);
  pinMode(rx, INPUT);
  // attaching a pin to the servo object that is defined previously
  door.attach(doorpin);
  // homing the door .. closing it.
  door.write(10);
}

void loop() {
  int i;
  delay(500);
  // if there is anything set via serial communication (check if there is any orders sent)
  if (Serial.available() > 0){
    // if there is then we want to run a function with it to know which order is it and act accordingly
    while (Serial.available() > 0){
      input[i] = Serial.read();
      i++;
    }
    // this is the function that deals with the order... go read it.
    Check_Protocol(input);
  }
  // read the temperature and if mode is analog then take actions according to the data you have. (the other mode).
  temp = analogRead(temp_pin);
  if (mode == 0){
    take_action(temp);
  }
}



// later on in this function we will replace the doorpin led with a servo and use the
// Servo library to open the door.
// Also we will use an lcd to desplay words for each command like "Wellcome User" or something and also display values like the temprature.

void Check_Protocol(char inStr[]){
  int i;
  int m;
  str_in = String(inStr);
  if (str_in.startsWith("pass=")){
    if(str_in == "pass="+password){
      if (door_state == 0){
        door.write(120);
        door_state = 1;
      }else{
        door.write(10);
        door_state = 0;
      }
      err_count = 0;
    }else{
      if (door_state == 0){
        err_count++;
      }else{
        door_state = 0;
        Serial.println("Wrong password.. but closing the door anyways");
        door.write(20);
      }
    }
    err(err_count);
  }else if (str_in ==  "mode"){
    if (mode == 1){
      mode = 0;
      Serial.println("Analog");
    }else{
      mode = 1;
      Serial.println("digital");
    }
  }else{
      if (str_in == "bed"){
        toggle(bed_pin);
      }else if (str_in == "mode"){
        if (mode == 0){
          mode = 1;
          Serial.println("user preference");
        }else{
          mode = 0;
          Serial.println("analog data");
        }
      }else if(str_in == "living"){
        toggle(living_pin);
      }else if (str_in == "fan"){
        toggle(fan); 
      }else{
        Serial.print("\"");
        Serial.print(str_in);
        Serial.println("\" is not an order");
      }
  }

  for(m=0;m<11;m++){
    inStr[m]=0;
  }
  str_in = String(inStr);
  i=0;
}


void take_action(int temp_val){
  if (temp_val > 40){
    digitalWrite(fan, HIGH);
  }else{
    digitalWrite(fan, LOW);
  }
  if (millis()/1000 - temp_timer > 1){
    temp_timer = millis() / 1000;
    Serial.print("Temperature is ");
    Serial.print(temp_val);
    Serial.print("\n");
  }

  if (digitalRead(pir_pin) == 1){
    pir_timer = millis() / 1000;
    digitalWrite(living_pin, HIGH);
  }else{
    if(millis()/1000 - pir_timer > delay_time){
      digitalWrite(living_pin, LOW);
    }
  }
}


void err(int count){
  if (count == 0){
//    digitalWrite(errpin, LOW);
    Serial.println("Hello user");
    digitalWrite(alarm_pin, LOW);
  }else if (count == 1){
    Serial.println("Wrong password");
  }else if(count == 2){
    Serial.println("Focus you got one more try");
  }else{
    Serial.println("you are a thief... run");
//    digitalWrite(errpin, HIGH);
    digitalWrite(alarm_pin, HIGH);
  }
}

void toggle(int pin){
  if (digitalRead(pin) == 1){
    digitalWrite(pin, LOW);
    Serial.print(pin);
    Serial.print(" OFF\n");
  }else{
    digitalWrite(pin, HIGH);
    Serial.print(pin);
    Serial.print(" ON\n");
  }
  mode = 1;
}











