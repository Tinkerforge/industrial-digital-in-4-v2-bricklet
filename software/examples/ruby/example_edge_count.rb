#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_industrial_digital_in_4_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
idi4 = BrickletIndustrialDigitalIn4V2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Configure rising edge count (channel 3) with 10ms debounce
idi4.set_edge_count_configuration 3, 0, 10

# Get edge count 10 times with 1s delay
for _ in 0..9
  sleep 1

  # Get current edge count
  count = idi4.get_edge_count 3, false
  puts "Count: #{count}"
end

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
