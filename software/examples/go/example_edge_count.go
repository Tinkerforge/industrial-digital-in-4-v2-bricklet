package main

import (
	"fmt"
	"time"
	"tinkerforge/industrial_digital_in_4_v2_bricklet"
	"tinkerforge/ipconnection"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	idi4, _ := industrial_digital_in_4_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Configure rising edge count (channel 3) with 10ms debounce
	idi4.SetEdgeCountConfiguration(3, 0, 10)

	// Get edge count 10 times with 1s delay
	for i := 0; i < 10; i++ {
		time.Sleep(1000 * time.Millisecond)

		// Get current edge count.
		count, _ := idi4.GetEdgeCount(3, false)
		fmt.Printf("Count: \n", count)
	}

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
