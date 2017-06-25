#define PIN_A 4
#define PIN_X 5
#define LED_1 2

#define PIN_B 16
#define PIN_Y 15
#define LED_2 0

#define PIN_C 13
#define PIN_Z 12
#define LED_3 14

bool input1;
bool input2;
bool input3;

unsigned long reset_ts;
unsigned long input1_high_ts;
unsigned long input2_high_ts;
unsigned long input3_high_ts;

void resetOutputs() {
  digitalWrite( PIN_A, HIGH );
  digitalWrite( PIN_B, LOW );
  digitalWrite( PIN_C, LOW );

  digitalWrite( LED_1, LOW );
  digitalWrite( LED_2, LOW );
  digitalWrite( LED_3, LOW );

  input1 = LOW;
  input2 = LOW;
  input3 = LOW;

  reset_ts = millis();
}

void setup() {
  pinMode( PIN_A, OUTPUT );
  pinMode( PIN_B, OUTPUT );
  pinMode( PIN_C, OUTPUT );

  pinMode( PIN_X, INPUT );
  pinMode( PIN_Y, INPUT );
  pinMode( PIN_Z, INPUT );

  pinMode( LED_1, OUTPUT );
  pinMode( LED_2, OUTPUT );
  pinMode( LED_3, OUTPUT );

  resetOutputs();

  Serial.begin(9600);
}

void read_pins() {
  input1 = digitalRead( PIN_X );
  delay( 10 );
  input2 = digitalRead( PIN_Y );
  delay( 10 );
  input3 = digitalRead( PIN_Z );
  delay( 10 );

  Serial.print( input1 );
  Serial.print( " " );
  Serial.print( input2 );
  Serial.print( " " );
  Serial.println( input3 );

  delay( 100 );
}

void loop() {
  read_pins();
  while ( not input1 ) {
    read_pins();
    input1_high_ts = millis();
  }

  if ( input1 and ( input1_high_ts - 500 ) > reset_ts and not input2 and not input3 ) {
    digitalWrite( LED_1, HIGH );
    digitalWrite( PIN_B, HIGH );

    read_pins();
    while ( input1 and not input2 and not input3 ) {
      read_pins();
      input2_high_ts = millis();
    }

    if ( input1 and input2 and ( input2_high_ts - 500 ) > reset_ts and not input3 ) {

      digitalWrite( LED_2, HIGH );
      digitalWrite( PIN_C, HIGH );

      read_pins();
      while ( input1 and input2 and not input3 ) {
        read_pins();
        input3_high_ts = millis();
      }

      if ( input1 and input2 and input3 and ( input3_high_ts - 500 ) > reset_ts ) {
        while ( input1 and input2 and input3 ) {
          for ( int i = 0; i < 9; i++ ) {
            digitalWrite( LED_3, HIGH );
            delay( 250 );
            digitalWrite( LED_3, LOW );
            delay( 250 );
          }

          delay( 1500 );
          read_pins();
        }
      }
    }
  }

  resetOutputs();
}
