#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIndustrialDigitalIn4V2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

# Callback subroutine for value callback
sub cb_value
{
    my ($channel, $changed, $value) = @_;

    print "Channel: $channel\n";
    print "Changed: $changed\n";
    print "Value: $value\n";
    print "\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $idi4 = Tinkerforge::BrickletIndustrialDigitalIn4V2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register value callback to subroutine cb_value
$idi4->register_callback($idi4->CALLBACK_VALUE, 'cb_value');

# Set period for value (channel 1) callback to 0.1s (100ms)
$idi4->set_value_callback_configuration(1, 100, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
