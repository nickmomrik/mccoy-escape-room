#include <Adafruit_CircuitPlayground.h>

float X, Y, Z;
int setupReds[5] = {255, 128, 0, 0, 128};
int setupGreens[5] = {0, 128, 255, 0, 0};
int setupBlues[5] = {0, 0, 0, 255, 128};

unsigned long last = 0;

void initialize() {
  for ( int i = 0; i < 5; i++ ) {
    for ( int j = 0; j < 10; j++ ) {
        CircuitPlayground.setPixelColor( j, setupReds[i], setupGreens[i], setupBlues[i] );
        delay( 30 );
        CircuitPlayground.clearPixels();
    }
    for ( int j = 0; j < 10; j++ ) {
      CircuitPlayground.setPixelColor( j, setupReds[i], setupGreens[i], setupBlues[i] );
      CircuitPlayground.playTone( 500, 10 );
    }
    delay( 80 );
    CircuitPlayground.clearPixels();
  }
}

void readAccel() {
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();
}

void success() {
  CircuitPlayground.clearPixels();

  for ( int i; i < 2; i++ ) {
    for ( int j = 0; j < 10; j++ ) {
      CircuitPlayground.setPixelColor( j, 255, 255, 255 );
      delay( 10 );
    }

    delay( 50 );

    CircuitPlayground.clearPixels();
    CircuitPlayground.playTone( 500, 10 );
  }

  last = millis();
}

bool notTooLong() {
  return last + 5000 > millis();
}

void win() {
  for ( int i = 0; i < 10; i++ ) {
    CircuitPlayground.playTone( 500, 50 );
    delay( 50 );
  }

  for ( int i = 0; i < 3; i++ ) {
    CircuitPlayground.setPixelColor( i, 128, 0, 128 );
  }

  while ( true ) {
    // Do nothing
  }
}

void setup() {
  CircuitPlayground.begin();

  delay( 2000 );

  initialize();
}

void loop() {
  readAccel();

  CircuitPlayground.clearPixels();
  CircuitPlayground.setPixelColor( 0, 255, 255, 255 );
  CircuitPlayground.setPixelColor( 9, 255, 255, 255 );

  if ( Z > 0 and abs( Y ) < 5 and X < -5 ) {
    success();

    CircuitPlayground.setPixelColor( 6, 255, 255, 255 );
    CircuitPlayground.setPixelColor( 7, 255, 255, 255 );
    CircuitPlayground.setPixelColor( 8, 255, 255, 255 );

    while ( notTooLong() ) {
      readAccel();
      if ( Z > 0 and abs( X ) < 5 and Y > 5 ) {
        success();

        CircuitPlayground.setPixelColor( 0, 255, 255, 255 );
        CircuitPlayground.setPixelColor( 9, 255, 255, 255 );

        while ( notTooLong() ) {
          readAccel();
          if ( Z > 0 and abs( Y ) < 5 and X < -5 ) {
            success();

            CircuitPlayground.setPixelColor( 1, 255, 255, 255 );
            CircuitPlayground.setPixelColor( 2, 255, 255, 255 );
            CircuitPlayground.setPixelColor( 3, 255, 255, 255 );

            while ( notTooLong() ) {
              readAccel();
              if ( Z > 0 and abs( X ) < 5 and Y < -5 ) {
                success();

                CircuitPlayground.setPixelColor( 1, 255, 255, 255 );
                CircuitPlayground.setPixelColor( 2, 255, 255, 255 );
                CircuitPlayground.setPixelColor( 3, 255, 255, 255 );
  
                while ( notTooLong() ) {
                  readAccel();
                  if ( Z > 0 and abs( X ) < 5 and Y < -5 ) {
                    success();
  
                    CircuitPlayground.setPixelColor( 4, 255, 255, 255 );
                    CircuitPlayground.setPixelColor( 5, 255, 255, 255 );
  
                    while ( notTooLong() ) {
                      readAccel();
                      if ( Z > 0 and abs( Y ) < 5 and X > 5 ) {
                        success();

                        CircuitPlayground.setPixelColor( 6, 255, 255, 255 );
                        CircuitPlayground.setPixelColor( 7, 255, 255, 255 );
                        CircuitPlayground.setPixelColor( 8, 255, 255, 255 );
                    
                        while ( notTooLong() ) {
                          readAccel();
                          if ( Z > 0 and abs( X ) < 5 and Y > 5 ) {
                            success();

                            CircuitPlayground.setPixelColor( 0, 255, 255, 255 );
                            CircuitPlayground.setPixelColor( 9, 255, 255, 255 );
                    
                            while ( notTooLong() ) {
                              readAccel();
                              if ( Z > 0 and abs( Y ) < 5 and X < -5 ) {
                                success();
                    
                                win();
                              }
                            }

                            break;
                          }

                          delay( 250 );
                        }

                        break;
                      }
  
                      delay( 250 );
                    }

                    break;
                  }
  
                  delay( 250 );
                }

                break;
              }

              delay( 250 );
            }

            break;
          }

          delay( 250 );
        }
      
        break;
      }

      delay( 250 );
    }

    initialize();
  }
}
