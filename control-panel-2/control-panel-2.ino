const byte ENABLE_PIN = 16;

const byte SWITCH_POWER_PIN = 5;

const byte BINARY2_PIN = 14;
const byte BINARY1_PIN = 0;
const byte BINARY0_PIN = 4;
const byte GREEN_PIN = 12;
const byte ENABLE_PANEL3_PIN = 13;

// Correct binary code (place from right to left)
bool binary2 = LOW;
bool binary1 = HIGH;
bool binary0 = HIGH;

// Status
bool correct = false;

void setup() {
  pinMode( ENABLE_PIN, INPUT );

  pinMode( SWITCH_POWER_PIN, OUTPUT );
  digitalWrite( SWITCH_POWER_PIN, LOW );

  pinMode( BINARY2_PIN, INPUT );
  pinMode( BINARY1_PIN, INPUT );
  pinMode( BINARY0_PIN, INPUT );

  pinMode( GREEN_PIN, OUTPUT );
  digitalWrite( GREEN_PIN, LOW );
  pinMode( ENABLE_PANEL3_PIN, OUTPUT );
  digitalWrite( ENABLE_PANEL3_PIN, LOW );

  Serial.begin(9600);
}

void loop() {
  int read_in = digitalRead( ENABLE_PIN );
  digitalWrite( SWITCH_POWER_PIN, read_in );
  Serial.print("Enable: ");
  Serial.println(read_in);

  if ( HIGH == read_in ) {
    read_in = digitalRead( BINARY0_PIN );
    if ( read_in == binary0 ) {
      read_in = digitalRead( BINARY1_PIN );
      if ( read_in == binary1 ) {
        read_in = digitalRead( BINARY2_PIN );
        if ( read_in == binary2 ) {
          if ( false == correct ) {
            digitalWrite( GREEN_PIN, HIGH );
            delay( 5000 );
            digitalWrite( GREEN_PIN, LOW );
    
            correct = true;
          }
        } else {
          correct = false;
        }
      } else {
        correct = false;
      }
    } else {
      correct = false;
    }  
  } else {
    correct = false;
  }

  digitalWrite( ENABLE_PANEL3_PIN, correct );
}
