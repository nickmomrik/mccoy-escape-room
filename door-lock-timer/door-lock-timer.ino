#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306();

// Pins
#define BTN1_PIN 6
#define BTN2_PIN 5
#define BUZZ_PIN 11

// Can be used to set the code back to all 0 when entered incorrectly
#define RESET_ON_FAIL false

// Choices for how long the timer should be
const byte minute_length = 3;
const byte minute_options[minute_length] = { 20, 30, 10 };

// Door lock code
const byte code_length = 6;
const byte combination[code_length] = { 1, 0, 0, 0, 0, 0 };

// Selected timer minutes
byte selected_minutes = 0;

// Active digit of the code
byte code_digit;
// The entered code
byte code[code_length] = {};

byte stage = 0;
/*
 * 0 - Waiting to start, change timer minutes
 * 1 - Alarm, then wait for timer start
 * 2 - Locked
 * 3 - Unlocked
 * 4 - FAIL
 */

// Millisecond value of when the timer started
unsigned long start_time = 0;
// When the display timer was last updated. Only update every second so there is no flickering
unsigned long timer_updated = 0;

// Timer ran out without code being unlocked. Stop everything.
bool died = false;

void setup() {
  // Get the display ready
  display.begin( SSD1306_SWITCHCAPVCC, 0x3C );
  display.display();
  delay( 1000 );
  display.clearDisplay();
  display.display();
  display.setTextSize( 1 );
  display.setTextColor( WHITE );

  // Setup the pins
  pinMode( BTN1_PIN, INPUT_PULLUP );
  pinMode( BTN2_PIN, INPUT_PULLUP );
  pinMode( BUZZ_PIN, OUTPUT );
  digitalWrite( BUZZ_PIN, LOW );
  
  reset_code();
  update_display();
}

void loop() {
  if ( 4 != stage ) {
    // Act if either button was pressed
    check_buttons();

    // Update every second for the timer
    if ( stage > 1 and timer_updated and ( millis() - timer_updated ) > 1000 ) {
      update_display();
    }
  } else if ( ! died ) {
    // Timer just ran out
    update_display();
    digitalWrite( BUZZ_PIN, HIGH );
    delay( 3000 );
    digitalWrite( BUZZ_PIN, LOW );

      died = true;
  }
}

// Set the code back to all 0s
void reset_code() {
  for ( int i = 0; i < code_length; i++ ) {
    code[i] = 0;
  }

  code_digit = 0;
}


// Generic function for displaying a list of selections and an underline to show which one is selected
void display_selections(
    byte display_width,     // Pixel width of the display
    byte char_width,        // Pixel width of a single character
    byte display_y,         // Pixel row where the selections will be printed
    byte selections[],      // Array of selections
    byte selections_length, // Number of items in the array
    byte selection_chars,   // How many characters is a selection
    byte selection_index    // Array index of the selected item
  ) {

  // Determine spacing for the selections
  byte side_margin = ( display_width % selections_length ) / 2;
  byte selection_space = ( display_width - ( 2 * side_margin ) ) / selections_length;
  byte selection_margin = ( selection_space - ( selection_chars * char_width ) ) / 2;

  // Display each selection
  for ( int i = 0; i < selections_length; i++ ) {
    display.setCursor( side_margin + selection_margin + ( i * selection_space ), display_y );
    display.print( selections[i] );
  }

  // Underline the selected item
  byte underscore_x = side_margin + selection_margin + ( selection_index * selection_space );
  for ( int i = 0; i < selection_chars; i++ ) {
    display.setCursor( underscore_x + ( i * char_width ), display_y + 4 );
    display.print( '_' );
  }

  display.display();
}

// Update the display with various messages depending on the stage
void update_display() {
  display.setCursor( 0, 0 );

  switch ( stage ) {
    case 0:
      display.clearDisplay();
      display.print( "<- Set timer mins\n-> Alarm, then Start" );
      display_selections( 128, 5, 21, minute_options, minute_length, 2, selected_minutes );
      break;
    case 1:
    case 2:
      display.clearDisplay();
      display.print( "LOCKED" );
      display_timer();
      display_selections( 128, 5, 21, code, code_length, 1, code_digit );
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

// Show the timer
void display_timer() {
  int elapsed = 0;
  int timer_seconds = 60 * minute_options[selected_minutes];

  // Upper right of the screen
  display.setCursor( 96, 0 );

  // Make sure the timer has started
  if ( start_time > 0 ) {
    elapsed = ( millis() - start_time ) / 1000;
  }

  if ( elapsed >= timer_seconds ) {
    // Time ran out
    stage = 4;
  } else {
    int minutes = ( timer_seconds - elapsed ) / 60;
    if ( minutes < 10 ) {
      display.print( '0' ); // Leading 0
    }
    display.print( minutes );
    display.print( ':' );
    int seconds = timer_seconds - elapsed - ( 60 * minutes );
    if ( seconds < 10 ) {
      display.print( '0' ); // Leading 0
    }
    display.print( seconds );
  }

  timer_updated = millis();
}

// Check to see if the buttons have been pressed
void check_buttons() {
  if ( 0 == stage or 2 == stage ) {
    bool btn1 = digitalRead( BTN1_PIN );
    if ( btn1 == LOW ) {
      // Wait for release, to prevent multiple presses
      while ( btn1 == LOW ) {
        btn1 = digitalRead( BTN1_PIN );
      }

      if ( 0 == stage ) {
        // Toggle which minutes value is selected
        selected_minutes++;

        if ( selected_minutes == minute_length ) {
          selected_minutes = 0;
        }
      } else {
        // Alter the number for the selected code digit
        if ( 9 == code[code_digit] ) {
          code[code_digit] = 0;
        } else {
          code[code_digit]++;
        }
      }

      update_display();
    }
  }

  bool btn2 = digitalRead( BTN2_PIN );
  if ( btn2 == LOW ) {
    // Wait for release, to prevent multiple presses
    while ( btn2 == LOW ) {
      btn2 = digitalRead( BTN2_PIN );
    }

    switch ( stage ) {
      case 0:
        // Sound the alarm!
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
        // Start the timer
        stage++;
        start_time = millis();

        update_display();
        break;
  
      case 2:
        // Toggle the active code digit or check the code at the end
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

// Determines if the code matches the combination
bool code_is_valid() {
  for ( int i = 0; i < code_length; i++ ) {
    if ( code[i] != combination[i] ) {
      // Buzz "womp-womp" type alert when wrong
      for ( int i = 0; i < 2; i++ ) {
        digitalWrite( BUZZ_PIN, HIGH );
        delay( 100 );
        digitalWrite( BUZZ_PIN, LOW );
        delay( 50 );
      }
  
      if ( RESET_ON_FAIL ) {
        reset_code();
      }

      return false;
    }
  }

  // Play a quick series of sounds for correct, as if the door is unlocking
  for ( int i = 0; i < 20; i++ ) {
    digitalWrite( BUZZ_PIN, HIGH );
    delay( 10 );
    digitalWrite( BUZZ_PIN, LOW );
    delay( 30 );
  }

  return true;
}

