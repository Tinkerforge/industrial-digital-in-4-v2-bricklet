function octave_example_edge_count()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    idi4 = javaObject("com.tinkerforge.BrickletIndustrialDigitalIn4V2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Configure rising edge count for channel 3 with 10ms debounce
    idi4.setEdgeCountConfiguration(3, 0, 10);

    % Get edge count 10 times with 1s delay
    for i = 0:9
        pause(1);

        % Get current edge count
        count = idi4.getEdgeCount(3, false);
        fprintf("Count: %d\n", java2int(count));
    end

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

function int = java2int(value)
    if compare_versions(version(), "3.8", "<=")
        int = value.intValue();
    else
        int = value;
    end
end
