// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_industrial_digital_in_4_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

static TF_IndustrialDigitalIn4V2 idi4;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_industrial_digital_in_4_v2_create(&idi4, NULL, hal), "create device object");

	// Configure rising edge count (channel 3) with 10ms debounce
	check(tf_industrial_digital_in_4_v2_set_edge_count_configuration(&idi4, 3, 0,
	                                                                 10), "call set_edge_count_configuration");

	// Get edge count 10 times with 1s delay
	int i;
	for (i = 0; i < 10; ++i) {
		tf_hal_sleep_us(hal, 1000 * 1000);

		// Get current edge count
		uint32_t count;
		check(tf_industrial_digital_in_4_v2_get_edge_count(&idi4, 3, false,
		                                                   &count), "get edge count");

		tf_hal_printf("Count: %I32u\n", count);
	}
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
