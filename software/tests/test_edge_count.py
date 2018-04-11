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

    # Set edge count configuration
    for i in range(0, 4):
        idi.set_edge_count_configuration(i, idi.EDGE_TYPE_BOTH, 128)

    # Get edge count configuration
    for i in range(0, 4):
        print "[+] CH", i, "CONFIG =", idi.get_edge_count_configuration(i)

    print ""

    time.sleep(8)

    # Report edge counts
    for i in range(0, 4):
        print "[+] CH", i, "EDGE COUNT =", idi.get_edge_count(i, True)

    print ""

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
