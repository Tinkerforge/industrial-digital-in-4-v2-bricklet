// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_industrial_digital_in_4_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

// Callback function for value callback
static void value_handler(TF_IndustrialDigitalIn4V2 *device, uint8_t channel,
                          bool changed, bool value, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	if (channel == TF_INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_0) {
		tf_hal_printf("Channel: 0\n");
	} else if (channel == TF_INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_1) {
		tf_hal_printf("Channel: 1\n");
	} else if (channel == TF_INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_2) {
		tf_hal_printf("Channel: 2\n");
	} else if (channel == TF_INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_3) {
		tf_hal_printf("Channel: 3\n");
	}

	tf_hal_printf("Changed: %s\n", changed ? "true" : "false");
	tf_hal_printf("Value: %s\n", value ? "true" : "false");
	tf_hal_printf("\n");
}

static TF_IndustrialDigitalIn4V2 idi4;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_industrial_digital_in_4_v2_create(&idi4, NULL, hal), "create device object");

	// Register value callback to function value_handler
	tf_industrial_digital_in_4_v2_register_value_callback(&idi4,
	                                                      value_handler,
	                                                      NULL);

	// Set period for value (channel 1) callback to 0.1s (100ms)
	tf_industrial_digital_in_4_v2_set_value_callback_configuration(&idi4, 1, 100, false);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
