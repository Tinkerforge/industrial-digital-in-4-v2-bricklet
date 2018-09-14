#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_industrial_digital_in_4_v2 import BrickletIndustrialDigitalIn4V2

# Callback function for value callback
def cb_value(channel, changed, value):
    print("Channel: " + str(channel))
    print("Changed: " + str(changed))
    print("Value: " + str(value))
    print("")

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    idi4 = BrickletIndustrialDigitalIn4V2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Register value callback to function cb_value
    idi4.register_callback(idi4.CALLBACK_VALUE, cb_value)

    # Set period for value (channel 1) callback to 0.1s (100ms)
    idi4.set_value_callback_configuration(1, 100, False)

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
