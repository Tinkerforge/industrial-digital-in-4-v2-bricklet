#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

# Configure rising edge count (channel 3) with 10ms debounce
tinkerforge call industrial-digital-in-4-v2-bricklet $uid set-edge-count-configuration channel-3 edge-type-rising 10

# Get edge count 10 times with 1s delay
for i in 0 1 2 3 4 5 6 7 8 9; do
	sleep 1

	# Get current edge count
	tinkerforge call industrial-digital-in-4-v2-bricklet $uid get-edge-count channel-3 false
done
