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
        // Get current value
        idi4.getValue(
            function (value) {
                console.log('Channel 0: ' + value[0]);
                console.log('Channel 1: ' + value[1]);
                console.log('Channel 2: ' + value[2]);
                console.log('Channel 3: ' + value[3]);
            },
            function (error) {
                console.log('Error: ' + error);
            }
        );
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
