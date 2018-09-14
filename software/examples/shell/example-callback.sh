#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

# Handle incoming value callbacks
tinkerforge dispatch industrial-digital-in-4-v2-bricklet $uid value &

# Set period for value (channel 1) callback to 0.1s (100ms)
tinkerforge call industrial-digital-in-4-v2-bricklet $uid set-value-callback-configuration 1 100 false

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
