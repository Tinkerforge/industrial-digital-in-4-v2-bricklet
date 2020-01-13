#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

import time

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_industrial_digital_in_4_v2 import BrickletIndustrialDigitalIn4V2

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    idi4 = BrickletIndustrialDigitalIn4V2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Configure rising edge count (channel 3) with 10ms debounce
    idi4.set_edge_count_configuration(3, 0, 10)

    # Get edge count 10 times with 1s delay
    for i in range(10):
        time.sleep(1)

        # Get current edge count
        count = idi4.get_edge_count(3, False)
        print("Count: " + str(count))

    input("Press key to exit\n") # Use raw_input() in Python 2
    ipcon.disconnect()
