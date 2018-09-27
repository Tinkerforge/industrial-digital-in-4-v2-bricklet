#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

# Get current value
tinkerforge call industrial-digital-in-4-v2-bricklet $uid get-value
