#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// Analog Pins
#define PHOTOCELL_PIN 0
#define POT_PIN 1

// Digital Pins
#define ULTRA_TRIG_PIN 12
#define ULTRA_ECHO_PIN 11
#define MAGNET_SWITCH_PIN 10
#define ENABLE_SWITCH_PIN 9

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix( 4, 8, 6,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800 );

const uint16_t orange = matrix.Color( 255, 69, 0 );
int photocell_read;
long ultrasonic_read;
bool magnet_read;
int pot_read;
bool enable_read;
int x = matrix.width();

void setup() {
  pinMode( ULTRA_TRIG_PIN, OUTPUT );
  pinMode( ULTRA_ECHO_PIN, INPUT );
  pinMode( MAGNET_SWITCH_PIN, INPUT );
  pinMode( ENABLE_SWITCH_PIN, INPUT );

  matrix.begin();
  matrix.setTextWrap( false );
  matrix.setBrightness( 40 );
  matrix.setTextColor( orange );

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

void loop() {
  enable_read = digitalRead( ENABLE_SWITCH_PIN );
  if ( enable_read ) {
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
  
    pot_read = analogRead( POT_PIN );
    Serial.print( "Pot: " );
    Serial.println( pot_read );
  
    Serial.println( "" );

    matrix.fillScreen( 0 );
    matrix.setCursor( x, 0 );
    matrix.print( F( "ORANGE=1  " ) );
    if ( --x < -48 ) {
      x = matrix.width();  
    }
    matrix.show();
  }

  delay( 100 );
}

