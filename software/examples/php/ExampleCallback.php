<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIndustrialDigitalIn4V2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIndustrialDigitalIn4V2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

// Callback function for value callback
function cb_value($channel, $changed, $value)
{
    echo "Channel: $channel\n";
    echo "Changed: $changed\n";
    echo "Value: $value\n";
    echo "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$idi4 = new BrickletIndustrialDigitalIn4V2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register value callback to function cb_value
$idi4->registerCallback(BrickletIndustrialDigitalIn4V2::CALLBACK_VALUE, 'cb_value');

// Set period for value (channel 1) callback to 0.1s (100ms)
$idi4->setValueCallbackConfiguration(1, 100, FALSE);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
