#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

# Handle incoming value callbacks
tinkerforge dispatch industrial-digital-in-4-v2-bricklet $uid value &

# Configure callback for channel 1 with fixed 100ms period
tinkerforge call industrial-digital-in-4-v2-bricklet $uid set-value-callback-configuration channel-1 100 false

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
