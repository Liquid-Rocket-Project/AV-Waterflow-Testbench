/*
 * Author: Nick Fan & Leo J. Che
 * Date: 2/15/2023
 * Revision Date: 3/6/2023
 * Revision 3
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

//define analog sensor pins
#define SENSOR0 A0
#define SENSOR1 A1
#define SENSOR2 A2
#define SENSOR3 A3
#define SENSOR4 A4
#define SENSOR5 A5
#define SENSOR6 A6
#define SENSOR7 A7


#define MAX_BUFFSIZE 50


const  float slope1 = 1.1306;
const  float intercept1 = -123.55 ;
const  float slope2 = 1.1328;
const  float intercept2 = -125.22;
const  float slope3 = 1.1259;
const  float intercept3 = -121.31;
const  float slope4 = 1.1345;
const  float intercept4 = -129.56;
const  float slope5 = 1.1386;
const  float intercept5 = -121.58;
const  float slope6 = 1;
const  float intercept6 = 0;
const  float slope7 = 1;
const  float intercept7 = 0;
const  float slope8 = 1;
const  float intercept8 = 0;


char uartBuffer[MAX_BUFFSIZE]; //buffer for storing data received from UART
float dataBuffer[MAX_BUFFSIZE];//buffer for storing data from analog pins
char displayBuffer[MAX_BUFFSIZE]; // buffer for displaying pin and state

byte errorFlag = LOW; //if error, set to HIGH to enter error handler
byte uartFlag = LOW; //flag to indicate a uart message was received
byte dataRdyFlag = LOW; //flag to indicate data is ready to send
volatile byte readFlag = LOW;

//Function declarations
void fillBuffer(char buffer[MAX_BUFFSIZE]);
void processBuffer(char uartBuffer[MAX_BUFFSIZE]);
void togglePin(int pin, char input, int state);
void error();
void getAnalogData(void);
void sendData(void);

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
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(PIN1, 0);
  digitalWrite(PIN2, 0);
  digitalWrite(PIN3, 0);
  digitalWrite(PIN4, 0);
  digitalWrite(PIN5, 0);

  //configure timer1 for 100ms interrupt for timing
  TCCR1A = 0;           //reset entire TCCR1A to 0
  TCCR1B = 0;           //reset entire TCCR1B to 0
  TCCR1B |= B00000100;  //Set CS12 to 1 so we get prescalar 256
  TIMSK1 |= B00000010; //Set OCIE1A to 1 so we enable compare match A
  OCR1A = 6250;         //Set Compare register A to needed value for 100ms
  
}


void loop() {
  // put your main code here, to run repeatedly:
  // enter if there is input from serial monitor, if no input it will not do anything
  
  //grab serial data
  fillBuffer(uartBuffer);
  //process it if new data was received
  if(uartFlag){processBuffer(uartBuffer);}
  if(readFlag){
    readFlag = LOW;
    if (pinState1||pinState2||pinState3||pinState4||pinState5) {
      getAnalogData();
      dataRdyFlag = HIGH;
      }
  }
  if(dataRdyFlag) {
    dataRdyFlag = LOW;
    digitalWrite(LED_BUILTIN, LOW);
    sendData();
  }
  
  if(errorFlag){error();}
}


//Functions

void fillBuffer(char buffer[MAX_BUFFSIZE]) {
  //Function to capture uart message and store it into a buffer
  static byte ndx = 0; //index of read operation
  char endMarker = '\n'; //denotes end of message
  char rc; //buffer for storing each character pulled out of hte buffer

  while(Serial.available()) {//perform actions if new serial data is available 
    rc = Serial.read();
    if(ndx >=MAX_BUFFSIZE) { //if buffer for receiving characters is exceeded
      errorFlag = HIGH; //denote error
      break; //exit loop
    }
    if (rc != endMarker) {
      buffer[ndx] = rc;
      
    }
    else {
      buffer[ndx] = '\0';
      ndx = 0;
      uartFlag = HIGH;
      break;
    }
    ndx++;
  }
}

void sendData(void){
  sprintf(displayBuffer, "%4d, %4d, %4d, %4d, %4d, %4d, %4d, %4d\n", (int)(dataBuffer[0]+.5), (int)(dataBuffer[1]+.5), (int)(dataBuffer[2]+.5), (int)(dataBuffer[3]+.5), (int)(dataBuffer[4]+.5), (int)(dataBuffer[5]+.5), (int)(dataBuffer[6]+.5), (int)(dataBuffer[7] +.5));
  Serial.print(displayBuffer);
  memset(displayBuffer, 0, MAX_BUFFSIZE);
}

void togglePin(int pin, char input, int state) {
/* This function toggles the pin, prints pin# and new state.
 *  @param pin: pin number
 *  @param input: the received input, just for display purposes
 *  @param state: the new state of the pin after toggling
 *  @return: None
 */
  digitalWrite(pin, state);
  sprintf(displayBuffer, "Toggle PIN%c %d\n", input, state);
  Serial.print(displayBuffer);
}

void processBuffer(char buffer[MAX_BUFFSIZE]) {
  char input;
  //processes the uart message and toggles the corresponding pins
  for (int i = 0; i < MAX_BUFFSIZE && i != '\n'; i++) {
    input = buffer[i];
    // Validates input and toggles any indicated pins
    switch(input) {
    case '1' :
      pinState1 = !pinState1;
      togglePin(PIN1, input, pinState1);

      break;
    case '2':
      pinState2 = !pinState2;
      togglePin(PIN2, input, pinState2);

      break;
    case '3':
      pinState3 = !pinState3;
      togglePin(PIN3, input, pinState3);

      break;
    case '4':
      pinState4 = !pinState4;
      togglePin(PIN4, input, pinState4);

      break;
    case '5':
      pinState5 = !pinState5;
      togglePin(PIN5, input, pinState5);

      break;

    default:
      break;
    }
    memset(&displayBuffer, 0, 20);
    input = 0; // reset input variable for next loop thru

  }
  
  memset(buffer, 0, MAX_BUFFSIZE);
  uartFlag = LOW;
}

void getAnalogData(void) {

  dataBuffer[0] = slope1*(float)analogRead(SENSOR0)+intercept1;
  dataBuffer[1] = slope2*(float)analogRead(SENSOR1)+intercept2;
  dataBuffer[2] = slope3*(float)analogRead(SENSOR2)+intercept3;
  dataBuffer[3] = slope4*(float)analogRead(SENSOR3)+intercept4;
  dataBuffer[4] = slope5*(float)analogRead(SENSOR4)+intercept5;
  dataBuffer[5] = slope6*(float)analogRead(SENSOR5)+intercept6;
  dataBuffer[6] = slope7*(float)analogRead(SENSOR6)+intercept7;
  dataBuffer[7] = slope8*(float)analogRead(SENSOR7)+intercept8;
}

ISR(TIMER1_COMPA_vect) {
  /*This is the timer timing when to make measurements*/
  TCNT1 = 0; //clear timer for next interrupt
  digitalWrite(LED_BUILTIN, HIGH);
  readFlag = HIGH;
}

void error(void) {
  Serial.println("Too many characters sent to buffer"); //send debug message
  errorFlag = LOW; //clear error
}
