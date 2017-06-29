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
#define PIEZO_PIN 5

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix( 4, 8, 6,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800 );

const uint16_t orange = matrix.Color( 255, 69, 0 );
int photocell_read;
bool photocell_good;
long ultrasonic_read;
bool ultrasonic_good;
bool magnet_read;
bool magnet_good;
int pot_read;
bool pot_good;
bool enable_read;
int x = matrix.width();

void clearMatrix() {
  matrix.fillScreen( 0 );
  matrix.show();
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

void soundTone() {
  tone( PIEZO_PIN, 1000, 100 );
}

void setup() {
  pinMode( ULTRA_TRIG_PIN, OUTPUT );
  pinMode( ULTRA_ECHO_PIN, INPUT );
  pinMode( MAGNET_SWITCH_PIN, INPUT );
  pinMode( ENABLE_SWITCH_PIN, INPUT );

  matrix.begin();
  matrix.setTextWrap( false );
  matrix.setBrightness( 40 );
  clearMatrix();
  matrix.setTextColor( orange );
}

void loop() {
  enable_read = digitalRead( ENABLE_SWITCH_PIN );
  if ( enable_read ) {
    for ( int y = 7; y >= 0; y-- ) {
      for ( int x = 0; x < 4; x++ ) {
        matrix.drawPixel( x, y, orange );
      }
      matrix.show();
      delay( 500 );
    }
    clearMatrix();
    delay( 500 );

    while ( enable_read ) {
      photocell_read = analogRead( PHOTOCELL_PIN );
      photocell_read = map( photocell_read, 650, 950, 0, 7 );
      if ( 7 == photocell_read ) {
        if ( ! photocell_good ) {
          soundTone();
        }
        photocell_good = true;
      } else {
        photocell_good = false;
      }
    
      ultrasonic_read = read_ultrasonic();
      ultrasonic_read = map( ultrasonic_read, 100, 4, 0, 7 );
      if ( 7 == ultrasonic_read ) {
        if ( ! ultrasonic_good ) {
          soundTone();
        }
        ultrasonic_good = true;
      } else {
        ultrasonic_good = false;
      }

      magnet_read = digitalRead( MAGNET_SWITCH_PIN );
      if ( magnet_read ) {
        if ( ! magnet_good ) {
          soundTone();
        }
        magnet_good = true;
      } else {
        magnet_good = false;
      }
    
      pot_read = analogRead( POT_PIN );
      pot_read = map( pot_read, 0, 1023, 0, 15 );
      pot_read = abs( 7 - pot_read );
      pot_read = map( pot_read, 7, 0, 0, 7 );
      if ( 7 == pot_read ) {
        if ( ! pot_good ) {
          soundTone();
        }
        pot_good = true;
      } else {
        pot_good = false;
      }
      
      if ( photocell_good and ultrasonic_good and magnet_good and pot_good ) {
        delay( 500 );
        for ( int i = 0; i < 52; i++ ) {
          matrix.fillScreen( 0 );
          matrix.setCursor( x, 0 );
          matrix.print( F( "ORANGE=1  " ) );
          if ( --x < -48 ) {
            x = matrix.width();  
          }
          matrix.show();
          delay( 150 );
        }
      } else {
        clearMatrix();
        for ( int i = photocell_read; i >= 0; i-- ) {
          matrix.drawPixel( 0, 7 - i, orange );
        }
        for ( int i = ultrasonic_read; i >= 0; i-- ) {
          matrix.drawPixel( 1, 7 - i, orange );
        }
        if ( magnet_read ) {
          for ( int i = 0; i < 8; i++ ) {
            matrix.drawPixel( 2, i, orange );
          }
        }
        for ( int i = pot_read; i >= 0; i-- ) {
          matrix.drawPixel( 3, 7 - i, orange );
        }
        matrix.show();
      }

      delay( 200 );
      enable_read = digitalRead( ENABLE_SWITCH_PIN );
    }

    clearMatrix();
  }

  delay( 500 );
}

