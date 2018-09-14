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
        // Configure rising edge count (channel 3) with 10ms debounce
        idi4.setEdgeCountConfiguration(3, 0, 10);

        // Get edge count 10 times with 1s delay
        for(var i = 0; i < 10; ++i) {
            setTimeout(function () {
                // Get current edge count
                idi4.getEdgeCount(3, false,
                    function (count) {
                        console.log('Count: ' + count);
                    },
                    function (error) {
                        console.log('Error: ' + error);
                    }
                );
            }, 1000 * i + 1000);
        }
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
