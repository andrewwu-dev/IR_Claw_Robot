/**
 * [IR_Claw_Robot.ino]
 * program for controlling arduino robot with infared sensor and SunFounder Roll Paw
 * @Authors: Andrew Wu & Siyao Chen
 * Date: January 23, 2018 
 * INSTRUCTIONS FOR REMOTE: 
 * '2' - move forward
 * '4' - turn left
 * '6' - turn right
 * '8' - move backward
 * '+' - increase claw frequency by 1
 * '-' - decrease claw frequency by 1
 */

//Libraries  
#include <IRremote.h>
#include <Servo.h>    // Create servo object to control a servo 

//declarations
#define E1 A0 //Enable pin for motor 1
#define E2 A1 //Enable pin for motor 2
#define I1 2 // Control pin 1 for motor 1
#define I2 3 // Control pin 2 for motor 1
#define I3 4 // Control pin 1 for motor 2
#define I4 5 // Control pin 2 for motor 2
//RollPaw
Servo Servo_0;
//Infrared Sensor
int IR = A5; //pin for Infrared Sensor
IRrecv receiver(IR);
decode_results output;
unsigned long key_value = 0;
//Variable for claw frequency
int clawFrequency = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //initialize IR
  receiver.enableIRIn();

  //initialize motor
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);
  pinMode(I1, OUTPUT);
  pinMode(I2, OUTPUT);
  pinMode(I3, OUTPUT);
  pinMode(I4, OUTPUT);

  //Attach the servos on pins to the servo object
  Servo_0.attach(6);
}

void loop() {
  // put your main code here, to run repeatedly:

  //if IR sensor reads input then decode
  if (receiver.decode(&output)) {
    //gets rid of unwanted inputs
    if (output.bits > 0) {
      //if button was held
      if (output.value == 0XFFFFFFFF) {
        output.value = key_value;
      }
      //switch case:
      switch (output.value) {
        // '-' button
        case 0xFFE01F:
          Serial.println("-");
           //prevent claw frequency from going out of 
          if (clawFrequency > 0) {
            clawFrequency--;//decrease
          }
          break ;
        // '+' button  
        case 0xFFA857:
          Serial.println("+");
          //prevent claw frequency from going out of bounds
          if (clawFrequency < 255) {
            clawFrequency++;//increase
          }
          break ;
        //'2' button FORWARD
        case 0xFF18E7:
          Serial.println("2");
          moveForward();
          break ;
        //'4' button LEFT  
        case 0xFF10EF:
          Serial.println("4");
          turnLeft();
          break ;
        //unused button
        case 0xFF38C7:
          Serial.println("5");
          break ;
        //'6' button  RIGHT
        case 0xFF5AA5:
          Serial.println("6");
          turnRight();
          break ;
        //'8' button  BACKWARD
        case 0xFF4AB5:
          Serial.println("8");
          moveBack();
          break ;
      }
      key_value = output.value;
    }
    //print
    Serial.println(output.value, HEX);
    //send frequency to claw
    Servo_0.write(clawFrequency);
    // receive the next value
    receiver.resume();
  }
}

/*
   moveForward
   method that makes the arduino bot go forward
*/
void moveForward() {
  //power the wheels
  analogWrite(E1, 1000);
  analogWrite(E2, 1000);

  //set car to move forward
  digitalWrite(I1, HIGH);
  digitalWrite(I2, LOW);
  digitalWrite(I3, LOW);
  digitalWrite(I4, HIGH);

  delay(1000);

  stopCar();
}//end of moveForward

/*
   turnRight
   method that turns arduino bot right
*/
void turnRight() {
  //power the wheels
  analogWrite(E1, 200);
  analogWrite(E2, 200);

  //set car to turn right
  digitalWrite(I1, HIGH);
  digitalWrite(I2, LOW);
  digitalWrite(I3, HIGH);
  digitalWrite(I4, LOW);

  delay(300);

  stopCar();
}//end of turnRight


/*
   turnLeft
   method that turns arduino bot left
*/
void turnLeft() {
  //power the wheels
  analogWrite(E1, 200);
  analogWrite(E2, 200);

  //set car to turn left
  digitalWrite(I1, LOW);
  digitalWrite(I2, HIGH);
  digitalWrite(I3, LOW);
  digitalWrite(I4, HIGH);

  delay(300);

  stopCar();
}//end of turnLeft

/*
   stop
   method that stops arduino bot from moving
*/
void stopCar() {
  //turn wheels off
  analogWrite(E1, 0);
  analogWrite(E2, 0);
}

/*
 * moveBack
 * method that allows arduino bot to move backwards
 */
void moveBack() {
  //send power to the wheels
  analogWrite(E1, 1000);
  analogWrite(E2, 2000);

  //set car to move forward
  digitalWrite(I1, LOW);
  digitalWrite(I2, HIGH);
  digitalWrite(I3, HIGH);
  digitalWrite(I4, LOW);

  
  delay(1000);

  stopCar();
}

