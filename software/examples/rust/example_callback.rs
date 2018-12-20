use std::{io, error::Error};
use std::thread;
use tinkerforge::{ip_connection::IpConnection, 
                  industrial_digital_in_4_v2_bricklet::*};


const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let idi4 = IndustrialDigitalIn4V2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
<<<<<<< Updated upstream
                                          // Don't use device before ipcon is connected.

    let value_receiver = idi4.get_value_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `idi4` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for value in value_receiver {
            if value.channel == INDUSTRIAL_DIGITAL_IN_4_V2_BRICKLET_CHANNEL_0 {
                println!("Channel: 0");
            } else if value.channel == INDUSTRIAL_DIGITAL_IN_4_V2_BRICKLET_CHANNEL_1 {
                println!("Channel: 1");
            } else if value.channel == INDUSTRIAL_DIGITAL_IN_4_V2_BRICKLET_CHANNEL_2 {
                println!("Channel: 2");
            } else if value.channel == INDUSTRIAL_DIGITAL_IN_4_V2_BRICKLET_CHANNEL_3 {
                println!("Channel: 3");
            }

            println!("Changed: {}", value.changed);
            println!("Value: {}", value.value);
            println!();
        }
    });

    // Set period for value (channel 1) callback to 0.1s (100ms).
    idi4.set_value_callback_configuration(1, 100, false);
=======
    // Don't use device before ipcon is connected.

     let value_receiver = idi4.get_value_callback_receiver();

        // Spawn thread to handle received callback messages. 
        // This thread ends when the `idi4` object
        // is dropped, so there is no need for manual cleanup.
        thread::spawn(move || {
            for value in value_receiver {           
                		if value.channel == INDUSTRIAL_DIGITAL_IN_4_V2_BRICKLET_CHANNEL_0 { 
			println!("Channel: 0");
		}
		else if value.channel == INDUSTRIAL_DIGITAL_IN_4_V2_BRICKLET_CHANNEL_1 { 
			println!("Channel: 1");
		}
		else if value.channel == INDUSTRIAL_DIGITAL_IN_4_V2_BRICKLET_CHANNEL_2 { 
			println!("Channel: 2");
		}
		else if value.channel == INDUSTRIAL_DIGITAL_IN_4_V2_BRICKLET_CHANNEL_3 { 
			println!("Channel: 3");
		}

		println!("Changed: {}", value.changed);
		println!("Value: {}", value.value);
		println!();
            }
        });

		// Set period for value (channel 1) callback to 0.1s (100ms).
		idi4.set_value_callback_configuration(1, 100, false);
>>>>>>> Stashed changes

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
