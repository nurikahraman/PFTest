/* <Controlling code for Arduino Controlled Rotary Stewart Platform>
Copyright (C) <2014>  <Tomas Korgo>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include <LiquidCrystal_SR3W.h>
#include <LiquidCrystal_SR2W.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal.h>
#include <I2CIO.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

//define of characters used for control of serial communication ['0'-'8']
#define SETBACKOFF 48		//0
#define SETBACKON 49		//1
#define SETPOSITIONS 50		//2
#define PRINTPOS 51			//3
#define STOPPRINTPOS 52		//4
#define SWITCHIRDA 53		//5
#define SETPOSITIONSINMS 54	//6
#define SWITCHIRDAOFF 55	//7
#define GEPOSITION 56		//8

//defines of LCD pin numbers, most probably they dont have to be changed except of I2C_ADDR which value is neccessary and have to be changed.
#define I2C_ADDR 0x27
#define LCD 1
#define BACKLIGHT_PIN 3
#define En 2
#define Rw 1
#define Rs 0
#define D4 4
#define D5 5
#define D6 6
#define D7 7

//variables used for proper show of positions on LCD
char shown = 0, showPos = 0, useIrda = 0;
unsigned long time;

//variable to store connected LCD
LiquidCrystal_I2C lcd(I2C_ADDR, En, Rw, Rs, D4, D5, D6, D7);


void setup() {
	//LCD inicialisation and turning on the backlight
#if LCD
	lcd.begin(16, 2);
	lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
	lcd.setBacklight(HIGH);
	lcd.home();
	lcd.print("6DOF LCD Test");
#endif

}


//functions used for displaying actual platform position on 16x2 LCD display
#if LCD
void showRot() {
	lcd.setCursor(0, 0);
	lcd.print("Rot");
	lcd.setCursor(12, 0);
	lcd.print((int)(10));
	lcd.setCursor(3, 1);
	lcd.print((int)(20));
	lcd.setCursor(11, 1);
	lcd.print((int)(30));
}
void showComm() {
	if (shown == 0) {
		shown = 1;
		lcd.setCursor(3, 0);
		lcd.print("ation x: ");
		lcd.setCursor(0, 1);
		lcd.print("y: ");
		lcd.setCursor(8, 1);
		lcd.print("z: ");
	}
}
void clearNr() {
	lcd.setCursor(12, 0);
	lcd.print("    ");
	lcd.setCursor(3, 1);
	lcd.print("     ");
	lcd.setCursor(11, 1);
	lcd.print("     ");

}
void showLoc() {
	lcd.setCursor(0, 0);
	lcd.print("Loc");
	lcd.setCursor(12, 0);
	lcd.print((int)(25.4));
	lcd.setCursor(3, 1);
	lcd.print((int)(25.4));
	lcd.setCursor(11, 1);
	lcd.print((int)(25.4));
}
#endif

//main control loop, obtain requested action from serial connection, then execute it
void loop()
{

	if (Serial.available()>0) {
		int input = Serial.read();
		//lcd.clear();

		//lcd.print(input);

		switch (input) {
			//action to turn backlight off
		case SETBACKOFF:
#if LCD
			lcd.setBacklight(LOW);
#endif
			break;
			//action to turn backlight on
		case SETBACKON:
#if LCD
			lcd.setBacklight(HIGH);
#endif
			break;
			//action to change position of platform, obtain 6 values representing desired position
		case SETPOSITIONS:
			//enable of showing current position on LCD
		case PRINTPOS:
#if LCD
			showPos = PRINTPOS;
			time = millis();
#endif
			break;
			//enable of controlling platformy by IrDA remote
		case SWITCHIRDA:
			/*#if IrDA
			irrecv.enableIRIn();
			useIrda=SWITCHIRDA;
			#endif
			*/
			break;
			//reserved for future use - possiblity to send just servo timing values
			//main control would be executed on communicating partner
		case SETPOSITIONSINMS:
			for (int i = 0; i<6; i++) {
				long kk;
				while (Serial.available()<4) {
					;
				}

			}
			break;
			//disable of showing current position on LCD
		case STOPPRINTPOS:
			showPos = STOPPRINTPOS;
			shown = 0;
			break;
			//disable of controlling platformy by IrDA remote
		case SWITCHIRDAOFF:
			useIrda = SWITCHIRDAOFF;
			break;
			//return current position of platform
		case GEPOSITION:
			break;
		default:
			break;
		}
	}
	//helping subroutine to print current position
#if LCD
	if (showPos == PRINTPOS) {
		static byte act = 0;
		showComm();
		if (millis() - time<1500) {
			act = 0;
		}
		else if (millis() - time<3000) {
			if (act == 0) {
				act = 1;
				clearNr();
				showRot();
			}
		}
		else {
			time = millis();
			clearNr();
			showLoc();
		}
	}
#endif
}

