#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_industrial_digital_in_4_v2.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

// Callback function for value callback
void cb_value(uint8_t channel, bool changed, uint8_t value, void *user_data) {
	(void)user_data; // avoid unused parameter warning

	printf("Channel: %u\n", channel);
	printf("Changed: %s\n", changed ? "true" : "false");
	printf("Value: %u\n", value);
	printf("\n");
}

int main(void) {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	IndustrialDigitalIn4V2 idi4;
	industrial_digital_in_4_v2_create(&idi4, UID, &ipcon);

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		return 1;
	}
	// Don't use device before ipcon is connected

	// Register value callback to function cb_value
	industrial_digital_in_4_v2_register_callback(&idi4,
	                                             INDUSTRIAL_DIGITAL_IN_4_V2_CALLBACK_VALUE,
	                                             (void *)cb_value,
	                                             NULL);

	// Configure callback for channel 1 with fixed 100ms period
	industrial_digital_in_4_v2_set_value_callback_configuration(&idi4, 1, 100, false);

	printf("Press key to exit\n");
	getchar();
	industrial_digital_in_4_v2_destroy(&idi4);
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	return 0;
}
