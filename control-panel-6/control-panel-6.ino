// Analog Pins
#define PHOTOCELL_PIN 0

// Digital Pins
#define ULTRA_TRIG_PIN 12
#define ULTRA_ECHO_PIN 11
#define MAGNET_SWITCH_PIN 10


void setup() {
  pinMode( ULTRA_TRIG_PIN, OUTPUT );
  digitalWrite( ULTRA_TRIG_PIN, LOW );
  pinMode( ULTRA_ECHO_PIN, INPUT );

  pinMode( MAGNET_SWITCH_PIN, INPUT );

  Serial.begin( 9600 );
}

long read_ultrasonic() {
  long ultra_duration;
  long ultra_distance;

  digitalWrite( ULTRA_TRIG_PIN, LOW );
  delay( 50 );
  digitalWrite( ULTRA_TRIG_PIN, HIGH );
  delay( 20 );
  digitalWrite( ULTRA_TRIG_PIN, LOW );

  ultra_duration = pulseIn( ULTRA_ECHO_PIN, HIGH );

  return ( ultra_duration / 2 ) / 29.1;
}

int photocell_read;
long ultrasonic_read;
bool magnet_read;

void loop() {
  photocell_read = analogRead( PHOTOCELL_PIN );
  Serial.print( "Photo: " );
  Serial.println( photocell_read );

  ultrasonic_read = read_ultrasonic();
  Serial.print( "Ultrasonic: " );
  Serial.print( ultrasonic_read );
  Serial.println( " cm" );

  magnet_read = digitalRead( MAGNET_SWITCH_PIN );
  Serial.print( "Magnet: " );
  Serial.println( magnet_read );

  Serial.println( "" );

  delay( 500 );
}

