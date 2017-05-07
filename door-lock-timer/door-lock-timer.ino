#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306();

#define BTN1_PIN 5
#define BTN2_PIN 6
#define BUZZ_PIN 11

const byte timer_minutes = 20;
const byte code_length = 6;
const byte combination[code_length] = { 1, 0, 0, 0, 0, 0 };

byte code[code_length] = {};

// Keeps track of which digit is selected
byte code_digit;

byte stage = 0;
/*
 * 0 - Waiting to start
 * 1 - Alarm, then wait for timer start
 * 2 - Locked
 * 3 - Unlocked
 * 4 - FAIL
 */

unsigned long start_time = 0;
unsigned long timer_updated = 0;

bool died = false;

void setup() {
  Serial.begin(9600);
  display.begin( SSD1306_SWITCHCAPVCC, 0x3C );

  display.display();
  delay( 1000 );
 
  // Clear the buffer.
  display.clearDisplay();
  display.display();
  
  pinMode( BTN1_PIN, INPUT_PULLUP );
  pinMode( BTN2_PIN, INPUT_PULLUP );

  pinMode( BUZZ_PIN, OUTPUT );
  digitalWrite( BUZZ_PIN, LOW );
  
  reset_code();

  display.setTextSize( 1 );
  display.setTextColor( WHITE );

  update_display();
}

void loop() {
  if ( 4 != stage ) {
    check_buttons();

    if ( stage > 1 and timer_updated and ( millis() - timer_updated ) > 1000 ) {
      update_display();
    }
  } else if ( ! died ) {
    update_display();
    died = true;
    digitalWrite( BUZZ_PIN, HIGH );
    delay( 3000 );
    digitalWrite( BUZZ_PIN, LOW );
  }
}

void reset_code() {
  for ( int i = 0; i < code_length; i++ ) {
    code[i] = 0;
  }

  code_digit = 0;
}

void update_display() {
  display.setCursor( 0, 0 );

  switch ( stage ) {
    case 0:
      display.clearDisplay();
      display.print( "Push 2nd button\nto trigger alarm.\n\nPush again to start." );
      break;
    case 1:
    case 2:
      display.clearDisplay();
      display.print( "LOCKED" );
      display_timer();
      display_code();
      break;

    case 3:
      display.clearDisplay();
      display.print( "UNLOCKED" );
      display.setCursor( 0, 16 );
      display.print( "CONGRATULATIONS!" );
      display.setCursor( 0, 24 );
      display.print( "YOU HAVE ESCAPED!" );
      break;

    case 4:
      display.clearDisplay();
      display.print( "TIMES UP..." );
      display.setCursor( 0, 24 );
      display.print( "YOU DIED!" );
    default:

      break;
  }

  display.setCursor( 0, 0 );
  display.display();
}

void display_timer() {
  int elapsed = 0;
  int timer_seconds = 60 * timer_minutes;
  
  display.setCursor( 96, 0 );

  if ( start_time > 0 ) {
    elapsed = ( millis() - start_time ) / 1000;
  }

  if ( elapsed >= timer_seconds ) {
    stage = 4;
  } else {
    int minutes = ( timer_seconds - elapsed ) / 60;
    if ( minutes < 10 ) {
      display.print( '0' );
    }
    display.print( minutes );
    display.print( ':' );
    int seconds = timer_seconds - elapsed - ( 60 * minutes );
    if ( seconds < 10 ) {
      display.print( '0' );
    }
    display.print( seconds );
  }

  timer_updated = millis();
}

void display_code() {
  byte side_margin = ( 128 % code_length ) / 2;
  byte char_space = ( 128 - ( 2 * side_margin ) ) / code_length;
  byte char_margin = ( char_space - 5 ) / 2;
  byte height = 21;

  for ( int i = 0; i < code_length; i++ ) {
    display.setCursor( side_margin + char_margin + ( i * char_space ), height );
    display.print( code[i] );
  }

  display.setCursor( side_margin + char_margin + ( code_digit * char_space ), height + 4 );
  display.print( '_' );

  display.display();
}

void check_buttons() {
  if ( 2 == stage ) {
    bool btn1 = digitalRead( BTN1_PIN );
    if ( btn1 == LOW ) {
      // Wait for release
      while ( btn1 == LOW ) {
        btn1 = digitalRead( BTN1_PIN );
      }
  
      if ( 9 == code[code_digit] ) {
        code[code_digit] = 0;
      } else {
        code[code_digit]++;
      }
      update_display();
    }
  }

  bool btn2 = digitalRead( BTN2_PIN );
  if ( btn2 == LOW ) {
    // Wait for release
    while ( btn2 == LOW ) {
      btn2 = digitalRead( BTN2_PIN );
    }

    switch ( stage ) {
      case 0:
        for ( int i = 0; i < 5; i++ ) {
          digitalWrite( BUZZ_PIN, HIGH );
          delay( 400 );
          digitalWrite( BUZZ_PIN, LOW );
          delay( 200 );
        }
        stage++;
        update_display();
        break;

      case 1:
        stage++;
        start_time = millis();

        update_display();
        break;
  
      case 2:
        code_digit++;

        if ( code_digit == code_length ) {
          if ( code_is_valid() ) {
            stage++;
          } else {
            code_digit = 0;
          }
        }

        update_display();

        break;

      case 3:
      default:
        break;
    }
  }
}

bool code_is_valid() {
  bool valid = true;

  for ( int i = 0; i < code_length; i++ ) {
    if ( code[i] != combination[i] ) {
      valid = false;
    }
  }

  if ( ! valid ) {
    for ( int i = 0; i < 2; i++ ) {
      digitalWrite( BUZZ_PIN, HIGH );
      delay( 100 );
      digitalWrite( BUZZ_PIN, LOW );
      delay( 50 );
    }

    reset_code();
  } else {
    for ( int i = 0; i < 20; i++ ) {
      digitalWrite( BUZZ_PIN, HIGH );
      delay( 10 );
      digitalWrite( BUZZ_PIN, LOW );
      delay( 30 );
    }
  }

  return valid;
}

