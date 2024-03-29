const byte BINARY2_PIN = 16;
const byte BINARY1_PIN = 4;
const byte BINARY0_PIN = 5;
const byte GREEN_PIN = 13;

// Correct binary code
// 011 = 3
bool binary2 = LOW;
bool binary1 = HIGH;
bool binary0 = HIGH;

void setup() {
  pinMode( BINARY2_PIN, INPUT );
  pinMode( BINARY1_PIN, INPUT );
  pinMode( BINARY0_PIN, INPUT );

  pinMode( GREEN_PIN, OUTPUT );
  digitalWrite( GREEN_PIN, LOW );
}

void loop() {
  bool correct = LOW;

  int read_in = digitalRead( BINARY0_PIN );
  if ( read_in == binary0 ) {
    read_in = digitalRead( BINARY1_PIN );
    if ( read_in == binary1 ) {
      read_in = digitalRead( BINARY2_PIN );
      if ( read_in == binary2 ) {
        correct = HIGH;
      }
    }
  }

  digitalWrite( GREEN_PIN, correct );

  delay( 50 );
}
