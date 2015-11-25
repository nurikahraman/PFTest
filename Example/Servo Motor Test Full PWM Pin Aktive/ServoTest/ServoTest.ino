/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 

#define MAX 2200
#define MIN 800

 
//Array of servo objects
Servo servo[6];


int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
 // myservo.attach(10);  // attaches the servo on pin 9 to the servo object 
   servo[0].attach(3, MIN, MAX);
   servo[1].attach(5, MIN, MAX);
   servo[2].attach(6, MIN, MAX);
   servo[3].attach(9, MIN, MAX);
   servo[4].attach(10, MIN, MAX);
   servo[5].attach(11, MIN, MAX);

  
} 
 
void loop() 
{ 
  for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
   // myservo.write(pos);// tell servo to go to position in variable 'pos'
   servo[0].write(pos);
   servo[1].write(pos);
   servo[2].write(pos);
   servo[3].write(pos);
   servo[4].write(pos);
   servo[5].write(pos);





     
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
   servo[0].write(pos);
   servo[1].write(pos);
   servo[2].write(pos);
   servo[3].write(pos);
   servo[4].write(pos);
   servo[5].write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
} 

