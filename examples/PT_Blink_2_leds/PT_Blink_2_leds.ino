#include <pt.h> // ProtoThread Library
#include <clock.h> // Library for the CLOCK_SECOND constant
#include <timer.h> // Libery for the timer

#define LEDPIN1 A1 // Definition of the led pin 1
#define LEDPIN2 A2 // Definition of the led pin 2

// Struct that keep a protothread instance
static struct pt pt1; // Instance of protothread for led 1
static struct pt pt2; // Instance of protothread for led 2

// Timer that will controle the time for the
// protothread instance
static struct timer t1; // Timer for the led 1
static struct timer t2; // Timer for the led 2

void setup(){
  // Change pinMode of led 1 and 2
  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  
  // Init the instances of protothreads
  PT_INIT(&pt1);
  PT_INIT(&pt2);
  
  // Set the time for each timer
  timer_set(&t1, 1*CLOCK_SECOND); // 1000ms for the led 1
  timer_set(&t2, 0.9*CLOCK_SECOND); // 900ms for the led 2
  
}

// Declaration of the Protothread
static PT_THREAD(pt_blinkLed(struct pt *pt, struct timer *t, char ledpin)){
  // This protothread have three parameters
  // The first one is the instance of protothread
  // The second one is the timer, to check if this timer is expired
  // The third one is the led pin, for the blink functions
  PT_BEGIN(pt); // Begin the protothread
  
  // while loop, like loop() of arduino
  while(1){
    // wait until the timer expire
    PT_WAIT_UNTIL(pt, timer_expired(t));
    // write HIGH to the led pin
    digitalWrite(ledpin, HIGH);
    // reset te timer to expire later
    timer_reset(t);
    // wait again the the timer expire
    PT_WAIT_UNTIL(pt, timer_expired(t));
    // write LOW to the led pin
    digitalWrite(ledpin, LOW);
    // reset the timer and start the loop again
    timer_reset(t);
  }
  
  PT_END(pt); // End of the protothread
}

void loop(){
  // The call of the protothread for led 1
  // loop that the instance is different from the second call
  // the timer is different too, as the led pin
  pt_blinkLed(&pt1, &t1, LEDPIN1);
  pt_blinkLed(&pt2, &t2, LEDPIN2);
  
  // The call of protothread functions should be periodically
  // the system won't call it automatically
}
