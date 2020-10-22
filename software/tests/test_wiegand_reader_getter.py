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
        print('Bad Length {0}'.format(len(data)))
        return

    pe = 0
    for i in range(0, 13):
        pe = pe ^ data[i]

    po = 0
    for i in range(13, 26):
        po = po ^ data[i]

    if pe != 0 or po != 1:
        print('Paritiy Error')
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

def cb_wiegand_error_count(framing_error_count, overflow_error_count):
    print('Framing Error Count {0}, Overflow Error Count {1}'.format(framing_error_count, overflow_error_count))

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    idi = BrickletIndustrialDigitalIn4V2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    idi.register_callback(idi.CALLBACK_WIEGAND_ERROR_COUNT, cb_wiegand_error_count)

    idi.set_wiegand_reader_config(True, 26, 50) # 26 bit, with 50 ms timeout
    idi.set_wiegand_callback_config(False, False, True)

    while True:
        time.sleep(0.1)

        data = idi.read_wiegand_data()

        if len(data) > 0:
            interpret_wiegand26_data(data)

    ipcon.disconnect()
