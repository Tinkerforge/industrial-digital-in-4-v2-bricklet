use std::{error::Error, io, thread};
use tinkerforge::{industrial_digital_in_4_v2_bricklet::*, ipconnection::IpConnection};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let industrial_digital_in_4_v2_bricklet = IndustrialDigitalIn4V2Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    //Create listener for value events.
    let value_listener = industrial_digital_in_4_v2_bricklet.get_value_receiver();
    // Spawn thread to handle received events. This thread ends when the industrial_digital_in_4_v2_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in value_listener {
            println!("Channel: {}", event.channel);
            println!("Changed: {}", event.changed);
            println!("Value: {}", event.value);
            println!();
        }
    });

    // Set period for value (channel 1) callback to 0.1s (100ms)
    industrial_digital_in_4_v2_bricklet.set_value_callback_configuration(1, 100, false);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
