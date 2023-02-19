/*
 * Author: Nick Fan
 * Date: 2/15/2023
 * Revision 2
 * For copy paste into Arduino IDE.
 * Note: Enter inputs in serial monitor. The first input after 
 * boot may not generate output. Make sure all output pins are 
 * tested prior to waterflow.
 */

// libraries to include
#include "stdio.h"
#include "string.h"

// defining Pinouts for GPIO output (Arduino Nano Pins)
#define PIN1 5 // D5
#define PIN2 6 // D6
#define PIN3 7 // D7
#define PIN4 8 // D8
#define PIN5 9 // D9
char input;

char displayBuffer[20]; // buffer for displaying pin and state

// variables to keep track of pin states
uint8_t pinState1 = LOW;
uint8_t pinState2 = LOW;
uint8_t pinState3 = LOW;
uint8_t pinState4 = LOW;
uint8_t pinState5 = LOW;

void setup() {
  // put your setup code here, to run once:
  // set baudrate for arduino IDE serial monitor
  Serial.begin(9600);

  // set pinmodes, default pins to low
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN4, OUTPUT);
  pinMode(PIN5, OUTPUT);
  digitalWrite(PIN1, 0);
  digitalWrite(PIN2, 0);
  digitalWrite(PIN3, 0);
  digitalWrite(PIN4, 0);
  digitalWrite(PIN5, 0);
  
}

/* This function toggles the pin, prints pin# and new state.
 *  @param pin: pin number
 *  @param input: the received input, just for display purposes
 *  @param state: the new state of the pin after toggling
 *  @return: None
 */
void togglePin(int pin, char input, int state) {
  digitalWrite(pin, state);
  sprintf(displayBuffer, "Toggle PIN%c %d\n", input, state);
  Serial.print(displayBuffer);
}

void loop() {
  // put your main code here, to run repeatedly:
  // enter if there is input from serial monitor, if no input it will not do anything
  if (Serial.available()) {
    input = Serial.read();
    // Validates input and toggles any indicated pins
    if (input == '1') {
      pinState1 = !pinState1;
      togglePin(PIN1, input, pinState1);
    }
    else if (input == '2') {
      pinState2 = !pinState2;
      togglePin(PIN2, input, pinState2);
    }
    else if (input == '3') {
      pinState3 = !pinState3;
      togglePin(PIN3, input, pinState3);
    }
    else if (input == '4') {
      pinState4 = !pinState4;
      togglePin(PIN4, input, pinState4);
    }
    else if (input == '5') {
      pinState5 = !pinState5;
      togglePin(PIN5, input, pinState5);
    }
    memset(&displayBuffer, 0, 20);
    input = 0; // reset input variable for next loop thru
  }
}