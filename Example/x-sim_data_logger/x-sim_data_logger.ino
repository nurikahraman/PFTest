/*
Opensource code for xsimulator by Jean David SEDRUE
Version betatest26 - 24042012
test21 with Tacho
test22 less tache to continue biMotor test
*/
#include <servo.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

//int n = 1;

LiquidCrystal_I2C	lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

#define MAX 2200
#define MIN 800

#define BRAKEVCC 0
#define RV  2 //according to cable at 24/11/2011
#define FW  1 //according to cable at 24/11/2011
#define STOP 0
#define BRAKEGND 3
#define CS_THRESHOLD 253 //not use :(
////////////////////////////////////////////////////////////////////////////////
#define pwmMin 128;
#define pwmMax 250;
#define motLeft 0
#define motRigth 1
#define speedPin 10
#define potL A1
#define potR A2

////////////////////////////////////////////////////////////////////////////////
//  DECLARATIONS
////////////////////////////////////////////////////////////////////////////////
/*  VNH2SP30 pin definitions*/
int inApin[2] = {
	7, 4 };  // INA: Clockwise input
int inBpin[2] = {
	8, 9 }; // INB: Counter-clockwise input
int pwmpin[2] = {
	5, 6 }; // PWM input
int cspin[2] = {
	2, 3 }; // CS: Current sense ANALOG input
int enpin[2] = {
	0, 1 }; // EN: Status of switches output (Analog pin)
int statpin = 13;
/* init position value*/
int DataValueL = 125; //middle position 
int DataValueR = 125; //middle position 

Servo servo[2];   //Array of servo objects
int pos = 0;    // variable to store the servo position 

				////////////////////////////////////////////////////////////////////////////////
				// INITIALIZATION 
				////////////////////////////////////////////////////////////////////////////////
void setup()
{


	lcd.begin(16, 2); //  <<----- My LCD was 16x2


					  // Switch on the backlight
	lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
	lcd.setBacklight(HIGH);
	lcd.home(); // go home

	lcd.print("x-sim data logger");

	/* serial initialization */
	Serial.begin(115200);


	servo[0].attach(3, MIN, MAX);
	servo[1].attach(5, MIN, MAX);


	/* motor initialization
	pinMode(statpin, OUTPUT);
	for (int i = 0; i<2; i++)
	{
	//pinMode(inApin[i], OUTPUT);
	//pinMode(inBpin[i], OUTPUT);
	pinMode(pwmpin[i], OUTPUT);
	}*/

}
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Main Loop ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void loop()
{
	int sensorL, sensorR;

	serialData();
	/*
	Serial.print(sensorL);
	Serial.print(" ; ");
	Serial.println(sensorR);
	motorGo(motRigth, FW, 255);
	delay(100);
	motorOff(motRigth);
	delay(1000);
	*/
	//motorMotion(motRigth, sensorR, DataValueR);
	//motorMotion(motLeft, sensorL, DataValueL);

}
////////////////////////////////////////////////////////////////////////////////
// Procedure: wait for complete trame
////////////////////////////////////////////////////////////////////////////////
void serialData()
{
	byte Data[5] = {
		'0','0','0','0','0' };
	// il faut limmitter le temps pour garder le contrôle des moteurs
	if (Serial.available()>2) { //wait a packets of 6
								//test if order  R or L data type
		Data[0] = Serial.read();
		if (Data[0] == 'R') { // ordre connue ?, si non je verrais au prochain tour !
			Data[1] = Serial.read();
			Data[2] = Serial.read();
			Data[3] = Serial.read();
			Data[4] = Serial.read();
			Data[5] = Serial.read();
			//delay(500);
			lcd.clear();
			//lcd.setCursor(0, 0);
			//lcd.print("R1");
			//lcd.setCursor(2, 0);
			//lcd.print("R3");
			//lcd.setCursor(8, 0);
			//lcd.print("R4");
			//lcd.setCursor(8, 0);
			//lcd.print("R5");


			lcd.setCursor(0, 1);
			lcd.print((char)Data[0]);
			lcd.setCursor(2, 1);
			lcd.print((char32_t)Data[2]);
			lcd.setCursor(8, 1);
			lcd.print((char32_t)Data[3]);
			lcd.setCursor(12, 1);
			lcd.print((char32_t)Data[4]);



			//DataValueR = NormalizeData(Data);
		}
		if (Data[0] == 'L') { // ordre connue ?, si non je verrais au prochain tour !
			Data[1] = Serial.read();
			Data[2] = Serial.read();
			Data[3] = Serial.read();
			Data[4] = Serial.read();
			Data[5] = Serial.read();
			//DataValueL = NormalizeData(Data);
			/*lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("L1");
			lcd.setCursor(2, 0);
			lcd.print("L2");
			lcd.setCursor(4, 0);
			lcd.print("L3");
			lcd.setCursor(6, 0);
			lcd.print("L4");
			lcd.setCursor(8, 0);
			lcd.print("L5");
			lcd.setCursor(12, 0);
			lcd.print("L6");*/

			lcd.setCursor(0, 0);
			lcd.print((char)Data[0]);
			lcd.setCursor(2, 0);
			lcd.print((char32_t)Data[2]);
			lcd.setCursor(8, 0);
			lcd.print((char32_t)Data[3]);
			lcd.setCursor(12, 0);
			lcd.print((char32_t)Data[4]);

		}
	}
	if (Serial.available()>16) Serial.flush();
}

/*
////////////////////////////////////////////////////////
void motorMotion(int numMot, int actualPos, int targetPos)
////////////////////////////////////////////////////////
{
//  int potPinL=A4; //to include in header procedure
//  int potPinR=A5; //to include in header procedure

int Tol = 4;
int difference;
int pwm;
int securityLow = 20;
int securityHigh = 200;
int brakeDistance = 16;
boolean move = true;

// encadrements des valeurs
if (targetPos<securityLow + brakeDistance)   targetPos = (securityLow + brakeDistance);    //software limits, but consideration speed brake.
if (targetPos>securityHigh - brakeDistance)  targetPos = (securityHigh - brakeDistance); //software limits, but consideration speed brake.
// calculate speed about distance
difference = abs(targetPos - actualPos);

if (difference <= Tol) pwm = 0;  // to near to mov
if (difference>Tol) pwm = 168;
if (difference>50) pwm = 200;
if (difference>75) pwm = 225;
if (difference>100) pwm = 255;
// pay attention for security for Right motor!!
if (actualPos>securityHigh || actualPos<securityLow) move = false; //mechanicals limits
//to near to move
if (difference <= Tol) move = false; //to near to move
// en avant   (haut)
if (actualPos<targetPos && move) {
pwm = 255; //need power to go up !
motorGo(numMot, FW, pwm);
}
pwm = 255;  //test !
// en arriere (bas)
if (actualPos>targetPos && move) motorGo(numMot, RV, pwm);
// ne pas bouger !
if (move == false) motorOff(numMot);// STOP
}
////////////////////////////////////////////////////////////////////////////////
void motorOff(int motor) {
////////////////////////////////////////////////////////////////////////////////
digitalWrite(inApin[motor], LOW);
digitalWrite(inBpin[motor], LOW);
analogWrite(pwmpin[motor], 0);
}
////////////////////////////////////////////////////////////////////////////////
void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)
////////////////////////////////////////////////////////////////////////////////
{
if (motor <= 1)
{
if (direct <= 4)
{
// Set inA[motor]
if (direct <= 1)
digitalWrite(inApin[motor], HIGH);
else
digitalWrite(inApin[motor], LOW);

// Set inB[motor]
if ((direct == 0) || (direct == 2))
digitalWrite(inBpin[motor], HIGH);
else
digitalWrite(inBpin[motor], LOW);

analogWrite(pwmpin[motor], pwm);

}
}
}
////////////////////////////////////////////////////////////////////////////////
// testPot
////////////////////////////////////////////////////////////////////////////////
void testPot() {

Serial.print(analogRead(A4));
Serial.print(";");
Serial.println(analogRead(A5));
delay(250);

}
////////////////////////////////////////////////////////////////////////////////
void testpulse() {
int pw = 120;
while (true) {

motorGo(motLeft, FW, pw);
delay(250);
motorOff(motLeft);
delay(250);
motorGo(motLeft, RV, pw);
delay(250);
motorOff(motLeft);

delay(500);

motorGo(motRigth, FW, pw);
delay(250);
motorOff(motRigth);
delay(250);
motorGo(motRigth, RV, pw);
delay(250);
motorOff(motRigth);
Serial.println("testpulse pwm:80");
delay(500);

}
}
*/
////////////////////////////////////////////////////////////////////////////////
// Function: convert Hex to Dec
////////////////////////////////////////////////////////////////////////////////
int NormalizeData(byte x[3])
////////////////////////////////////////////////////////////////////////////////
{
	int result;

	if (x[2] == 13)   //only a LSB and Carrier Return
	{
		x[2] = x[1];  //move MSB to LSB
		x[1] = '0';     //clear MSB
	}
	for (int i = 1; i<3; i++)
	{
		if (x[i]>47 && x[i]<58) {//for xA to xF
			x[i] = x[i] - 48;
		}
		if (x[i]>64 && x[i]<71) {//for x0 to x9
			x[i] = x[i] - 55;
		}
	}
	result = (x[1] * 16 + x[2]);
	return result;
}
