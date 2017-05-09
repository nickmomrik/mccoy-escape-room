#!/usr/bin/python

# Enable switch
switch_pin = 4

# Potentiameter in/out
pot1_pin = 20
pot2_pin = 21

# Button
button_pin = 27

# Shared 7 Segment Display Pins (A, B, C, D, E, F, G, DP)
segment_pins = [ 18, 23, 24, 25, 8, 7, 12, 16 ]

# Individual 7 Segment Displays
digit_pins = [ 22, 5, 6, 13, 19, 26 ]

############
# END CONFIG
############

import RPi.GPIO as GPIO
import time

# Setup the pins
GPIO.setmode( GPIO.BCM )
GPIO.setup( switch_pin,  GPIO.IN, pull_up_down = GPIO.PUD_DOWN )  #enable switch
GPIO.setup( button_pin, GPIO.IN, pull_up_down = GPIO.PUD_UP )  #button
GPIO.setup( segment_pins, GPIO.OUT, initial = GPIO.LOW )
GPIO.setup( digit_pins, GPIO.OUT, initial = GPIO.LOW )

# Various states
enabled = False
digit_values = [ 0, 0, 0, 0, 0, 0 ]
next_phase = False
selected_digit = 0

# Setup pot for capacitor data
def discharge_pot() :
  GPIO.setup( pot1_pin, GPIO.IN )
  GPIO.setup( pot2_pin, GPIO.OUT, initial = GPIO.LOW )
  time.sleep( 0.005 )

# Capture analog value
def pot_charge_time() :
  GPIO.setup( pot2_pin, GPIO.IN )
  GPIO.setup( pot1_pin, GPIO.OUT, initial = GPIO.HIGH )

  start = time.clock()
  while ( not GPIO.input( pot2_pin ) ) :
    pass

  return int( round( ( time.clock() - start ) * 1000 ) )

# Reading charging and discharging data
def read_pot() :
  discharge_pot()

  return pot_charge_time()

time.clock()

try :
  while ( True ) :
    while ( not enabled ) :
      if ( GPIO.HIGH == GPIO.input( switch_pin ) ) :
        enabled = True
        print "Switch enabled"

    if ( GPIO.LOW == GPIO.input( switch_pin ) ) :
      enabled = False
      if ( next_phase ) :
        next_phase = False
    else :
      pot = read_pot()
      #print "pot: " + str( pot )
      #selected_digit = map

      if ( not GPIO.input( button_pin ) ) :
        if ( selected_digit < len( digit_values ) ) :
          selected_digit += 1
        else :
          selected_digit = 0
        #while ( not GPIO.input( button_pin ) ) :
        #  pass
        print "button: " + str( selected_digit )

      for i in range( len( digit_values ) ) :
        for j in range( 7 ) :
          GPIO.output( segment_pins[j], GPIO.HIGH )
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
