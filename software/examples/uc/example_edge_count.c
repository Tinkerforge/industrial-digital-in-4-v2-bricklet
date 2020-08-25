#include "bindings/hal_common.h"
#include "bindings/bricklet_industrial_digital_in_4_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

void check(int rc, const char* msg);

TF_IndustrialDigitalIn4V2 idi4;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_industrial_digital_in_4_v2_create(&idi4, UID, hal), "create device object");


	// Configure rising edge count (channel 3) with 10ms debounce
	check(tf_industrial_digital_in_4_v2_set_edge_count_configuration(&idi4, 3, 0,
	                                                                 10), "call set_edge_count_configuration");

	// Get edge count 10 times with 1s delay
	int i;
	for(i = 0; i < 10; ++i) {
		tf_hal_sleep_us(hal, 1000 * 1000);

		// Get current edge count
		uint32_t count;
		check(tf_industrial_digital_in_4_v2_get_edge_count(&idi4, 3, false,
		                                                   &count), "get edge count");

		tf_hal_printf("Count: %u\n", count);
	}

}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
