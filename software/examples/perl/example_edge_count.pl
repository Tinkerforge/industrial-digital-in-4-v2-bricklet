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

# Configure rising edge count (channel 3) with 10ms debounce
$idi4->set_edge_count_configuration(3, 0, 10);

# Get edge count 10 times with 1s delay
for (my $i = 0; $i < 10; $i++)
{
    sleep(1);

    # Get current edge count
    my $count = $idi4->get_edge_count(3, 0);
    print "Count: $count\n";
}

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
