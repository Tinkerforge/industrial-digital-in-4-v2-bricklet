use std::{io, error::Error};
use std::thread;
use std::time::Duration;
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

		// Configure rising edge count (channel 3) with 10ms debounce
		idi4.set_edge_count_configuration(3, 0, 10);

		// Get edge count 10 times with 1s delay
		for i in 0..10{
			thread::sleep(Duration::from_millis(1000));

			// Get current edge count.
	let count = idi4.get_edge_count(3, false).recv()?;
			println!("Count: {}", count);
		}

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
