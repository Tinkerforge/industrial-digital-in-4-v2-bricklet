var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var idi4 = new Tinkerforge.BrickletIndustrialDigitalIn4V2(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Set period for value (channel 1) callback to 0.1s (100ms)
        idi4.setValueCallbackConfiguration(1, 100, false);
    }
);

// Register value callback
idi4.on(Tinkerforge.BrickletIndustrialDigitalIn4V2.CALLBACK_VALUE,
    // Callback function for value callback
    function (channel, changed, value) {
        if(channel === Tinkerforge.BrickletIndustrialDigitalIn4V2.CHANNEL_0) {
            console.log('Channel: 0');
        }
        else if(channel === Tinkerforge.BrickletIndustrialDigitalIn4V2.CHANNEL_1) {
            console.log('Channel: 1');
        }
        else if(channel === Tinkerforge.BrickletIndustrialDigitalIn4V2.CHANNEL_2) {
            console.log('Channel: 2');
        }
        else if(channel === Tinkerforge.BrickletIndustrialDigitalIn4V2.CHANNEL_3) {
            console.log('Channel: 3');
        }

        console.log('Changed: ' + changed);
        console.log('Value: ' + value);
        console.log();
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
