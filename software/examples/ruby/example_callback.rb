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

# Register value callback
idi4.register_callback(BrickletIndustrialDigitalIn4V2::CALLBACK_VALUE) do |channel,
                                                                           changed, value|
  puts "Channel: #{channel}"
  puts "Changed: #{changed}"
  puts "Value: #{value}"
  puts ''
end

# Configure callback for channel 1 with fixed 100ms period
idi4.set_value_callback_configuration 1, 100, false

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
