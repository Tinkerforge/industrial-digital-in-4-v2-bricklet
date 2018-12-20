use std::{io, error::Error};

use tinkerforge::{ip_connection::IpConnection, 
                  industrial_digital_in_4_v2_bricklet::*};


const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let idi4 = IndustrialDigitalIn4V2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
    // Don't use device before ipcon is connected.

		// Get current value.
let value = idi4.get_value().recv()?;

		println!("Channel 0: {}", value[0]);
		println!("Channel 1: {}", value[1]);
		println!("Channel 2: {}", value[2]);
		println!("Channel 3: {}", value[3]);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
