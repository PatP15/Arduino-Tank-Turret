
#include <Wire.h>
#include <PS2X_lib.h>  //for v1.6
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <Servo.h> 


Adafruit_MotorShield AFMS = Adafruit_MotorShield();   //  MotorShield v2

PS2X ps2x; // create PS2 Controller Class
int laserPin = 3;
int servo1_position = 0 ;
int error = 0; 
byte type = 0;
byte vibrate = 0;
const int trigPin = 16;
const int echoPin = 17;
// defines variables
long duration;
int distance;

Adafruit_DCMotor *rightmotor = AFMS.getMotor(2);   //Define DC motor name
Adafruit_DCMotor *leftmotor = AFMS.getMotor(1);
Adafruit_DCMotor *shooterone = AFMS.getMotor(3);
Adafruit_DCMotor *shootertwo = AFMS.getMotor(4);
Servo servo1;               // Define  servo name


void setup(){
 Serial.begin(9600);
 Serial.println("Ready to Rumble");
 
 AFMS.begin();  // create with the default frequency 1.6KHz
 

 error == ps2x.config_gamepad(8,2,12,6, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
 servo1.attach (10);
 
 if(error == 0){
  Serial.println("Found Controller, configured successful");
  Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
  Serial.println("holding L1 or R1 will print out the analog stick values.");
  Serial.println("Go to www.billporter.info for updates and to report bugs.");
 }
   
  else if(error == 1)
   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   
  else if(error == 3)
   Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
   
   Serial.print(ps2x.Analog(1), HEX);
   
   type = ps2x.readType(); 
     switch(type) {
       case 0:
        Serial.println("Unknown Controller type");
       break;
       case 1:
        Serial.println("DualShock Controller Found");
       break;
       case 2:
         Serial.println("GuitarHero Controller Found");
       break;
     } 
     pinMode (laserPin, OUTPUT); // define the digital output interface 13 feet
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop(){
  
 if(error == 1) //skip loop if no controller found
  return; 
      
 ps2x.read_gamepad();          //read controller and set large motor to spin at 'vibrate' speed
 
  digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);



  
 //  Using Analog sticks move left side and right side of robot independantly
 // Uncomment code below to use leftjoystick to control the motor instead of servo
 
int joyValueLeft = scaleForMotor(ps2x.Analog(PSS_LY));
 Serial.print("Left: ");                 // Only for serial monitor debugging
 Serial.println(ps2x.Analog(PSS_LY),DEC);
 
 if (joyValueLeft > 50){
   leftmotor->setSpeed(joyValueLeft);
   leftmotor->run(FORWARD);
   
 }
 
 else if (joyValueLeft < -50){
   leftmotor->setSpeed(joyValueLeft * -1);
   leftmotor->run(BACKWARD);
   
 }
 
 else{
 leftmotor->setSpeed(0);
leftmotor->run(RELEASE);
 
 }
  
 int joyValueRight = scaleForMotor(ps2x.Analog(PSS_RY));//the joyvalueright actually controls the left joystick
 Serial.print("Right: ");
 Serial.println(ps2x.Analog(PSS_RY),DEC);
 
  if (joyValueRight > 50){
   rightmotor->setSpeed(joyValueRight);
   rightmotor->run(BACKWARD);
   
 }
 
 else if (joyValueRight < -50){
   rightmotor->setSpeed(joyValueRight * -1);
   rightmotor->run(FORWARD);
   
 }
 
 else{
 rightmotor->setSpeed(0);
 rightmotor->run(RELEASE);
 
 } 
 
 if (ps2x.Button(PSB_TRIANGLE)) //Triangle to move tank forward
     { 
      rightmotor->setSpeed(150);
       leftmotor->setSpeed(150);
       leftmotor->run(BACKWARD);
       rightmotor->run(FORWARD);
       Serial.print("triangle");
     } 
else if (ps2x.Button(PSB_BLUE)) //X to move tank backwards
     {
      leftmotor->setSpeed(200);
     rightmotor->setSpeed(200);
       rightmotor->run(BACKWARD);
       leftmotor->run(FORWARD); //FORWARD for left motor is actually BACKWARD
       Serial.print("x");
     }
   
     
     else if (ps2x.Button(PSB_CIRCLE)) //Circle to turn tank right
     {
       leftmotor->setSpeed(150);
       leftmotor->run(FORWARD);
       Serial.print("circle");
     }
      else if (ps2x.Button(PSB_SQUARE)) //Square to turn tank left
     {
       rightmotor->setSpeed(150);
       rightmotor->run(FORWARD);
       
     }
      else if (ps2x.Button(PSB_R1))
    {
      shooterone->setSpeed(150);
      shooterone->run(BACKWARD);
    }
    else if (ps2x.Button(PSB_L1))
    {
      shootertwo->setSpeed(150);
      shootertwo->run(BACKWARD);
    }
    else if (ps2x.Button(PSB_L2))
    {
      rightmotor->run(RELEASE);
      leftmotor->run(RELEASE);
      shooterone->run(RELEASE);
      shootertwo->run(RELEASE);
    }
    
     if (ps2x.Button(PSB_PAD_DOWN)) {
   digitalWrite (laserPin, LOW); // Turn Laser Off
   Serial.print("OFF");
   
  
   }
   else if (ps2x.Button(PSB_PAD_UP)) {
   digitalWrite (laserPin, HIGH); // Turn Laser On
   Serial.print("ON");
   }  
   


  servo1.write(map(ps2x.Analog(PSS_LX), 0, 90, 0, 70));
delay(5);


if (ps2x.Button(PSB_SELECT))//change to autonomous mode
{
      
 if (distance < 12) 
     {
    
    Serial.print ("Turn Around!");
    leftmotor->setSpeed(200);
       leftmotor->run(BACKWARD);
        rightmotor->setSpeed(100);
       rightmotor->run(BACKWARD);
       delay(1000);
     }
     
 else if (distance > 12){
      //motors go forward
      leftmotor->setSpeed(120);
       leftmotor->run(FORWARD);
        rightmotor->setSpeed(120);
       rightmotor->run(BACKWARD);
       Serial.print("Autonomous mode!");
     }

   else {
    leftmotor->setSpeed(0);
    rightmotor->setSpeed(0);
   }
}
}
int scaleForMotor(int joyValue){
      
      int scaledVal = map(joyValue, 255, 0, -200, 200);
      Serial.print(scaledVal, DEC);
      Serial.print(", ");

      return scaledVal;
}
