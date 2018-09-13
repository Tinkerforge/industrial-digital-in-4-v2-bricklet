function matlab_example_input()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIndustrialDigitalIn4V2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    idi4 = handle(BrickletIndustrialDigitalIn4V2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current value
    value = idi4.getValue();

    fprintf('Channel 0: %i\n', value(1));
    fprintf('Channel 1: %i\n', value(2));
    fprintf('Channel 2: %i\n', value(3));
    fprintf('Channel 3: %i\n', value(4));

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
