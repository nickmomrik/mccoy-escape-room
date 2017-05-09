#!/usr/bin/python

# Enable switch
switch_pin = 4

# Buttons
button1_pin = 20
button2_pin = 27

# Next Phase
next_phase_pin = 21

# Shared 7 Segment Display Pins (A, B, C, D, E, F, G, DP)
segment_pins = [ 18, 23, 24, 25, 8, 7, 12, 16 ]

# Individual 7 Segment Displays
digit_pins = [ 22, 5, 6, 13, 19, 26 ]

# Correct code
secret_code = [ 1, 0, 0, 0, 0, 0 ]

############
# END CONFIG
############

import RPi.GPIO as GPIO
import time

# Segment on/off for each number
numbers = {
  0: ( 1, 1, 1, 1, 1, 1, 0 ),
  1: ( 0, 1, 1, 0, 0, 0, 0 ),
  2: ( 1, 1, 0, 1, 1, 0, 1 ),
  3: ( 1, 1, 1, 1, 0, 0, 1 ),
  4: ( 0, 1, 1, 0, 0, 1, 1 ),
  5: ( 1, 0, 1, 1, 0, 1, 1 ),
  6: ( 1, 0, 1, 1, 1, 1, 1 ),
  7: ( 1, 1, 1, 0, 0, 0 ,0 ),
  8: ( 1, 1, 1, 1, 1, 1, 1 ),
  9: ( 1, 1, 1, 1, 0, 1, 1 )
}

# Setup the pins
GPIO.setmode( GPIO.BCM )
GPIO.setup( switch_pin, GPIO.IN, pull_up_down = GPIO.PUD_DOWN )
GPIO.setup( button1_pin, GPIO.IN, pull_up_down = GPIO.PUD_UP )
GPIO.setup( button2_pin, GPIO.IN, pull_up_down = GPIO.PUD_UP )
GPIO.setup( next_phase_pin, GPIO.OUT, initial = GPIO.LOW )
GPIO.setup( segment_pins, GPIO.OUT, initial = GPIO.LOW )
GPIO.setup( digit_pins, GPIO.OUT, initial = GPIO.LOW )

# Various states
enabled = False
digit_values = [ 0, 0, 0, 0, 0, 0 ]
next_phase = False
selected_digit = 0

def reset() :
  enabled = False
  next_phase = False
  selected_digit = 0
  for i in range( len( digit_values ) ) :
    digit_values[i] = 0

def code_is_valid() :
  for i in range( len( digit_values ) ) :
    if ( not secret_code[i] == digit_values[i] ) :
      return False

  return True

time.clock()

try :
  while ( True ) :
    while ( not enabled ) :
      if ( GPIO.HIGH == GPIO.input( switch_pin ) ) :
        enabled = True
        print "Switch enabled"

    if ( GPIO.LOW == GPIO.input( switch_pin ) ) :
      reset()
    else :
      GPIO.output( next_phase_pin, next_phase )

      if ( not GPIO.input( button1_pin ) ) :
        current_value = digit_values[selected_digit]
        if ( 9 == current_value ) :
          digit_values[selected_digit] = 0
        else :
          digit_values[selected_digit] += 1

        next_phase = code_is_valid()

      if ( not GPIO.input( button2_pin ) ) :
        if ( selected_digit < len( digit_values ) ) :
          selected_digit += 1
        else :
          selected_digit = 0

      for i in range( len( digit_values ) ) :
        for j in range( 7 ) :
          GPIO.output( segment_pins[j], numbers[ digit_values[i] ][j] )
        if ( i == selected_digit ) :
          GPIO.output( segment_pins[7], GPIO.HIGH )
        else :
          GPIO.output( segment_pins[7], GPIO.LOW )

        GPIO.output( digit_pins[i], GPIO.HIGH )
        time.sleep( 0.0005 )
        GPIO.output( digit_pins[i], GPIO.LOW )

except KeyboardInterrupt :
  print 'Exiting via CTRL+C...'
finally :
  GPIO.cleanup()
