#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your IO-4 Bricklet

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_industrial_digital_in_4_v2 import BrickletIndustrialDigitalIn4V2

def cb_input_value(channel, changed, value):
    print "[+] INPUT VALUE CHANGED CB: CH =", channel, \
          "CHANGED =", changed, \
          "VALUE =", value, "\n"

def cb_all_input_value(changed, value):
    print "[+] ALL INPUT VALUE CHANGED CB: CHANGED =", changed, \
          "VALUE =", value, "\n"

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    idi = BrickletIndustrialDigitalIn4V2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Set input value callback configuration (value has to change)
    for i in range(0, 4):
        idi.set_input_value_callback_configuration(i, 100, True)

    idi.set_all_input_value_callback_configuration(500, True)
    
    # Get input value callback configuration
    for i in range(0, 4):
        print "[+] CH =", i, \
              " CONFIG =", idi.get_input_value_callback_configuration(i)

    print "[+] ALL CH CONFIG =", idi.get_all_input_value_callback_configuration(), "\n"

    # Register callbacks
    idi.register_callback(idi.CALLBACK_INPUT_VALUE, cb_input_value)
    idi.register_callback(idi.CALLBACK_ALL_INPUT_VALUE, cb_all_input_value)

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
