// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_industrial_digital_in_4_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);


static TF_IndustrialDigitalIn4V2 idi4;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_industrial_digital_in_4_v2_create(&idi4, UID, hal), "create device object");

	// Get current value
	bool value[4];
	check(tf_industrial_digital_in_4_v2_get_value(&idi4, value), "get value");

	tf_hal_printf("Channel 0: %s\n", value[0] ? "true" : "false");
	tf_hal_printf("Channel 1: %s\n", value[1] ? "true" : "false");
	tf_hal_printf("Channel 2: %s\n", value[2] ? "true" : "false");
	tf_hal_printf("Channel 3: %s\n", value[3] ? "true" : "false");
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
