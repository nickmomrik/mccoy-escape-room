#!/usr/bin/python

# Enable switch (also connected to 3v3)
switch_pin = 22

# Buttons
button1_pin = 21
button2_pin = 20

# Next Phase
next_phase_pin = 26

# Shared 7 Segment Display Pins (A, B, C, D, E, F, G, DP)
segment_pins = [ 4, 3, 14, 15, 18, 17, 27, 2 ]

# Individual 7 Segment Displays
digit_pins = [ 23, 24, 25, 8, 12, 16 ]

# Correct code
secret_code = [ 1, 0, 2, 0, 3, 0 ]

############
# END CONFIG
############

from gpiozero import LED, Button, DigitalOutputDevice
from time import sleep

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

# Various states
enabled = False
digit_values = [ 5, 5, 5, 5, 5, 5 ]
next_phase = False
selected_digit = 0

def reset() :
  global enabled, next_phase, next_phase_out, selected_digit, digit_values

  next_phase = False
  next_phase_out.off()

  selected_digit = 0
  for i in range( len( digit_values ) ) :
    digit_values[i] = 5

def switch_on() :
  global enabled

  if ( not enabled ) :
	reset()

  enabled = True

def switch_off() :
  global enabled

  if ( enabled ) :
	reset()

  enabled = False

def button1_action() :
  global selected_digit, digit_values

  current_value = digit_values[selected_digit]
  if ( 9 == current_value ) :
    digit_values[selected_digit] = 0
  else :
    digit_values[selected_digit] += 1

  check_code()

def button2_action() :
  global selected_digit, digit_values

  if ( selected_digit == len( digit_values ) - 1 ) :
    selected_digit = 0
  else :
    selected_digit += 1

  check_code()

def check_code() :
  global digit_values, secret_code, next_phase, next_phase_out

  valid = True

  for i in range( len( digit_values ) ) :
    if ( not secret_code[i] == digit_values[i] ) :
      valid = False

  next_phase = valid
  if ( next_phase ) :
	next_phase_out.on()
  else :
	next_phase_out.off()

def update_displays() :
  global digits, numbers, digit_values, segment_leds, selected_digit

  for i in range( len( digits ) ) :
    for j in range( 7 ) :
      if ( numbers[ digit_values[i] ][j] ) :
        segment_leds[j].on()
      else :
        segment_leds[j].off()

    if ( i == selected_digit ) :
      segment_leds[7].on()
    else :
      segment_leds[7].off()

    digits[i].on()
    sleep( 0.0005 )
    digits[i].off()

# Setup gpiozero
switch = Button( switch_pin, False )
button1 = Button( button1_pin )
button2 = Button( button2_pin )

switch.when_pressed = switch_on
switch.when_released = switch_off

button1.when_pressed = button1_action
button2.when_pressed = button2_action

next_phase_out = DigitalOutputDevice( next_phase_pin )

segment_leds = []
for i in range( len( segment_pins ) ) :
  segment_leds.append( LED( segment_pins[i] ) )

digits = []
for i in range( len( digit_pins ) ) :
  digits.append( LED( digit_pins[i] ) )

# Make sure initial state is correct
if ( switch.is_pressed ) :
  switch_on()

while ( True ) :
  if ( enabled ) :
  	update_displays()
