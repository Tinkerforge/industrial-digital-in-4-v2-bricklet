<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIndustrialDigitalIn4V2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIndustrialDigitalIn4V2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$idi4 = new BrickletIndustrialDigitalIn4V2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Configure rising edge count for channel 3 with 10ms debounce
$idi4->setEdgeCountConfiguration(3, 0, 10);

// Get edge count 10 times with 1s delay
for($i = 0; $i < 10; $i++) {
    sleep(1);

    // Get current edge count
    $count = $idi4->getEdgeCount(3, FALSE);
    echo "Count: $count\n";
}

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
