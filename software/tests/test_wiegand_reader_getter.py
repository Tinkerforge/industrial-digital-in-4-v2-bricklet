#!/usr/bin/env python3
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "HXg"

import time
from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_industrial_digital_in_4_v2 import BrickletIndustrialDigitalIn4V2

def interpret_wiegand26_data(data):
    if len(data) != 26:
        print('Bad length: {0}'.format(len(data)))
        return

    fc = 0
    for i in range(1, 9):
        fc = fc << 1
        fc = fc | data[i]

    cc = 0
    for i in range(9, 25):
        cc = cc << 1
        cc = cc | data[i]

    print('Facility Code {0}, Card Code {1}'.format(fc, cc))


def cb_wiegand_data_available():
    data = idi.read_wiegand_data()
    if len(data) > 0:
        interpret_wiegand26_data(data)

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    idi = BrickletIndustrialDigitalIn4V2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    idi.set_wiegand_reader_config(True, 26, 50)

    idi.set_wiegand_callback_config(False, True, False)
    idi.register_callback(idi.CALLBACK_WIEGAND_DATA_AVAILABLE, cb_wiegand_data_available)

#    while True:
#        time.sleep(0.1)
#        data = idi.read_wiegand_data()
#        if len(data) > 0:
#            interpret_wiegand26_data(data)

    input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
