#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIndustrialDigitalIn4V2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $idi4 = Tinkerforge::BrickletIndustrialDigitalIn4V2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current value
my $value = $idi4->get_value();

print "Channel 0: " . @{$value}[0] . "\n";
print "Channel 1: " . @{$value}[1] . "\n";
print "Channel 2: " . @{$value}[2] . "\n";
print "Channel 3: " . @{$value}[3] . "\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
