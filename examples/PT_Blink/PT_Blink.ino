#include <pt.h> // ProtoThread Library
#include <clock.h> // Library for the CLOCK_SECOND constant
#include <timer.h> // Libery for the timer

#define LEDPIN 13 // Definition of the led pin that will blink

// Struct that keep a protothread instance
static struct pt pt1;

void setup(){
  // Change pinMode of led to OUTPUT
  pinMode(LEDPIN, OUTPUT);
  // Init the instance of pt1
  PT_INIT(&pt1);
}

// Declaration of the Protothread
static PT_THREAD(protothread1(struct pt *pt)){
  static struct timer t; // Struct that will control the time
                         // of the protothread
                         // The static keyword is necessary to
                         // don't lose the data after the thread
                         // is yield
  PT_BEGIN(pt); // The begin of the protothread
                // All variables won't be saved if it isn't static
  
  // Set the timer to 1 second
  timer_set(&t, 1*CLOCK_SECOND);
  
  // while loop, like the loop() of arduino
  while(1){
    // If the timer is expired, then continue the function
    // otherwhise it'll pass the processing to another thread
    PT_WAIT_UNTIL(pt, timer_expired(&t));
    // write HIGH to the led pin
    digitalWrite(LEDPIN, HIGH);
    // reset the timer, to 1 second after the last expired time
    timer_reset(&t);
    // Wait again for the expire of the timer
    PT_WAIT_UNTIL(pt, timer_expired(&t));
    // write LOW to the led pin
    digitalWrite(LEDPIN, LOW);
    // reset the timer and start the loop again
    timer_reset(&t);
  }
  
  PT_END(pt); // The end of the protothread
}

void loop(){
  protothread1(&pt1); // Call of the protothread is
                      // necessary to make this call
                      // periodically, the system won't
                      // call it automatically
  
  // All protothreads should be called were
  // Any delay function or function call that keep the
  // process time for a while may disturb the protothreads
  // functionality
}
