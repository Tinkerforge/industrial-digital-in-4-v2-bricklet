package main

import (
	"fmt"
	"github.com/tinkerforge/go-api-bindings/industrial_digital_in_4_v2_bricklet"
	"github.com/tinkerforge/go-api-bindings/ipconnection"
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

	idi4.RegisterValueCallback(func(channel industrial_digital_in_4_v2_bricklet.Channel, changed bool, value bool) {

		if channel == industrial_digital_in_4_v2_bricklet.Channel0 {
			fmt.Println("Channel: 0")
		} else if channel == industrial_digital_in_4_v2_bricklet.Channel1 {
			fmt.Println("Channel: 1")
		} else if channel == industrial_digital_in_4_v2_bricklet.Channel2 {
			fmt.Println("Channel: 2")
		} else if channel == industrial_digital_in_4_v2_bricklet.Channel3 {
			fmt.Println("Channel: 3")
		}

		fmt.Printf("Changed: %s\n", changed)
		fmt.Printf("Value: %s\n", value)
		fmt.Println()
	})

	// Set period for value (channel 1) callback to 0.1s (100ms).
	idi4.SetValueCallbackConfiguration(1, 100, false)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
