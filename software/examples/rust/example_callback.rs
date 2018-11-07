use std::{error::Error, io, thread};
use tinkerforge::{industrial_digital_in_4_v2_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let idi4 = IndustrialDigitalIn4V2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    let value_receiver = idi4.get_value_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `idi4` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for value in value_receiver {
            println!("Channel: {}", value.channel);
            println!("Changed: {}", value.changed);
            println!("Value: {}", value.value);
            println!();
        }
    });

    // Set period for value (channel 1) callback to 0.1s (100ms).
    idi4.set_value_callback_configuration(1, 100, false);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
