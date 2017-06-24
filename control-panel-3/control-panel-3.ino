#include <IRrecv.h>

#define CODE_4 0x1DE20DF

const uint16_t IR_PIN = 4;
const byte WHITE_PIN = 14;
const byte RED_PIN = 5;

IRrecv irrecv( IR_PIN );

decode_results results;

void setup() {
  pinMode( WHITE_PIN, OUTPUT );
  pinMode( RED_PIN, OUTPUT );

  irrecv.enableIRIn();
}

void loop() {
  if ( irrecv.decode( &results ) ) {
    if ( results.value == CODE_4 ) {
      for ( int i; i < 4; i++ ) {
        digitalWrite( RED_PIN, HIGH );
        delay( 250 );
        digitalWrite( RED_PIN, LOW );
        delay( 250 );
      }
    } else {
      digitalWrite( WHITE_PIN, HIGH );
      delay( 25 );
      digitalWrite( WHITE_PIN, LOW );
    }

    irrecv.resume();
  }
}

