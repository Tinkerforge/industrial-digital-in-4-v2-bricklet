#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your IO-4 Bricklet

import time
from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_industrial_digital_in_4_v2 import BrickletIndustrialDigitalIn4V2

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    idi = BrickletIndustrialDigitalIn4V2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    print "[+] CHANNEL VALUES =", idi.get_value(), "\n"

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
