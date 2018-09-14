function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIndustrialDigitalIn4V2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    idi4 = handle(BrickletIndustrialDigitalIn4V2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register value callback to function cb_value
    set(idi4, 'ValueCallback', @(h, e) cb_value(e));

    % Set period for value (channel 1) callback to 0.1s (100ms)
    idi4.setValueCallbackConfiguration(1, 100, false);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for value callback
function cb_value(e)
    fprintf('Channel: %i\n', e.channel);
    fprintf('Changed: %i\n', e.changed);
    fprintf('Value: %i\n', e.value);
    fprintf('\n');
end
