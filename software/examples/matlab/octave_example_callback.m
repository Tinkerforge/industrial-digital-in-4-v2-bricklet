function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    idi4 = javaObject("com.tinkerforge.BrickletIndustrialDigitalIn4V2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register value callback to function cb_value
    idi4.addValueCallback(@cb_value);

    % Set period for value (channel 1) callback to 0.1s (100ms)
    idi4.setValueCallbackConfiguration(1, 100, false);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for value callback
function cb_value(e)
    if e.channel == 0
        fprintf("Channel: 0\n");
    elseif e.channel == 1
        fprintf("Channel: 1\n");
    elseif e.channel == 2
        fprintf("Channel: 2\n");
    elseif e.channel == 3
        fprintf("Channel: 3\n");
    end

    fprintf("Changed: %d\n", e.changed);
    fprintf("Value: %d\n", e.value);
    fprintf("\n");
end
