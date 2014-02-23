#include <pt.h> // ProtoThread Library
#include <clock.h> // Library for the CLOCK_SECOND constant
#include <timer.h> // Libery for the timer

#define LEDPIN 13 // Definition of the led that will blink

// Struct that keep a protothread instance
static struct pt ptLed; // instance for the blink protothread
static struct pt ptSerial; // instance for the serial protothread

void setup(){
  // Init both instances
  PT_INIT(&ptLed);
  PT_INIT(&ptSerial);
}

// Declaration of protothread that will blink
static PT_THREAD(ledBlink(struct pt *pt)){
  static struct timer t; // timer of protothread
  PT_BEGIN(pt);
  
  pinMode(LEDPIN, OUTPUT); // set led pin to ouput
  timer_set(&t, 1*CLOCK_SECOND); // set the timer to expire
                                 // in 1 second
  
  // while loop, like loop() of arduino
  while(1){
    // wait until the timer expire
    PT_WAIT_UNTIL(pt, timer_expired(&t));
    // write HIGH to the led pin
    digitalWrite(LEDPIN, HIGH);
    // reset te timer to expire later
    timer_reset(&t);
    // wait again the the timer expire
    PT_WAIT_UNTIL(pt, timer_expired(&t));
    // write LOW to the led pin
    digitalWrite(LEDPIN, LOW);
    // reset the timer and start the loop again
    timer_reset(&t);
  }
  
  PT_END(pt); // End of the protothread
}

static PT_THREAD(delaySerial(struct pt *pt)){
  static struct timer t; // Timer for the delayed serial
  static char c; // variable to hold the char value
                 // Pay attention to the static keyword,
                 // this keyword will prevent the data loss
                 // after the protothred is yield.
                 // Any data that has to be keeped should have
                 // static keyword
  PT_BEGIN(pt); // Begin the protothread
  
  Serial.begin(9600); // Start the Serial Port
  
  while(1){
    // Wait for any data in Serial Port
    PT_WAIT_UNTIL(pt, Serial.available());
    // Read one byte from the Serial Port
    c = Serial.read();
    // wait 250ms before continue the code
    timer_set(&t, 0.25*CLOCK_SECOND);
    PT_WAIT_UNTIL(pt, timer_expired(&t));
    // print the byte in the Serial Port
    Serial.print(c);
  }
  
  PT_END(pt); // End of the protothread
}

void loop(){
  // The calls of both protothreads
  ledBlink(&ptLed);
  delaySerial(&ptSerial);
  
  // The call of protothread functions should be periodically
  // the system won't call it automatically
}
