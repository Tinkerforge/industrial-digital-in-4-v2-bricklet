/* industrial-digital-in-4-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * idi4.c: Implementation of Industrial Digital In V2 Bricklet
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "idi4.h"

#include "bricklib2/logging/logging.h"
#include "bricklib2/hal/system_timer/system_timer.h"

#include "communication.h"

IDI4_t idi4;

void idi4_init() {
	logd("[+] IDI4: idi4_init()\n\r");

	XMC_GPIO_CONFIG_t gpio_config;

	// Initial channel config
	for(uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
		switch (i) {
			case 0:
				// Channel input 0: P2_2
				idi4.channels[i].pin = 2;

				// Info LED 0: P0_5
				idi4.info_leds[i].port_base = (XMC_GPIO_PORT_t *)PORT0_BASE;
				idi4.info_leds[i].pin = 5;

				break;

			case 1:
				// P2_6
				idi4.channels[i].pin = 6;

				// Info LED 1: P0_0
				idi4.info_leds[i].port_base = (XMC_GPIO_PORT_t *)PORT0_BASE;
				idi4.info_leds[i].pin = 0;

				break;

			case 2:
				// P2_7
				idi4.channels[i].pin = 7;

				// Info LED 2: P1_0
				idi4.info_leds[i].port_base = (XMC_GPIO_PORT_t *)PORT1_BASE;
				idi4.info_leds[i].pin = 0;

				break;

			case 3:
				// P2_9
				idi4.channels[i].pin = 9;

				// Info LED 3: P1_1
				idi4.info_leds[i].port_base = (XMC_GPIO_PORT_t *)PORT1_BASE;
				idi4.info_leds[i].pin = 1;

				break;

			default:
				break;
		}

		// Input channel init (as input tristate)
		gpio_config.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
		idi4.channels[i].port_base = (XMC_GPIO_PORT_t *)PORT2_BASE;
		gpio_config.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD;

		XMC_GPIO_Init(idi4.channels[i].port_base,
					  idi4.channels[i].pin,
					  &gpio_config);

		if(i == 2) {
			gpio_config.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
			gpio_config.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD;

			XMC_GPIO_Init(P2_8, &gpio_config);
		}

		// Store initial channel value
		idi4.channels[i].value = !((bool)XMC_GPIO_GetInput(idi4.channels[i].port_base, idi4.channels[i].pin));

		// Info LED init
		idi4.info_leds[i].info_led_flicker_state.config = LED_FLICKER_CONFIG_OFF;
		idi4.info_leds[i].config = INDUSTRIAL_DIGITAL_IN_4_V2_INFO_LED_CONFIG_SHOW_CHANNEL_STATUS;

		if(idi4.channels[i].value) {
			// Input channel has logic high
			gpio_config.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW; // Info LED on
		}
		else {
			// Input channel has logic low
			gpio_config.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH; // Info LED off
		}

		gpio_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL;

		XMC_GPIO_Init(idi4.info_leds[i].port_base,
					  idi4.info_leds[i].pin,
					  &gpio_config);
		
		// Edge count init
		idi4.channels[i].edge_count.last_value = idi4.channels[i].value;
		idi4.channels[i].edge_count.debounce = 100;
		idi4.channels[i].edge_count.edge_type = INDUSTRIAL_DIGITAL_IN_4_V2_EDGE_TYPE_RISING;
		idi4.channels[i].edge_count.cnt_edge_rising = 0;
		idi4.channels[i].edge_count.cnt_edge_falling = 0;
		idi4.channels[i].edge_count.debounce_start = system_timer_get_ms();

		// Input value CB init (disabled)
		idi4.channels[i].input_value_cb.period = 0;
		idi4.channels[i].input_value_cb.last_value = idi4.channels[i].value;
		idi4.channels[i].input_value_cb.period_start = 0;
		idi4.channels[i].input_value_cb.value_has_to_change = false;

		ringbuffer_init(&idi4.input_value_cb_rb,
		                INPUT_VALUE_CB_BUFFER_SIZE,
		                &idi4.input_value_cb_buffer[0]);

		// All input value CB init (disabled)
		idi4.all_input_value_cb.period = 0;
		idi4.all_input_value_cb.period_start = 0;
		idi4.all_input_value_cb.value_has_to_change = false;
		idi4.all_input_value_cb.last_values[i] = idi4.channels[i].value;

		ringbuffer_init(&idi4.all_input_value_cb.cb_rb,
		                ALL_INPUT_VALUE_CB_BUFFER_SIZE,
		                &idi4.all_input_value_cb.cb_buffer[0]);
	}
}

void idi4_tick(void) {
	uint8_t all_channel_values = 0;
	uint8_t all_channel_changed = 0;
	bool all_ch_in_value_changed = false;
	bool all_ch_in_period_expired = false;

	// Check if all channel input value callback is enabled and if period expired
	if((idi4.all_input_value_cb.period > 0) && (system_timer_is_time_elapsed_ms(idi4.all_input_value_cb.period_start, idi4.all_input_value_cb.period))) {
		all_ch_in_period_expired = true;
	}

	// Iterate over channels
	for(uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
		// Update current channel value
		idi4.channels[i].value = !((bool)XMC_GPIO_GetInput(idi4.channels[i].port_base, idi4.channels[i].pin));

		// Manage channel specific input value callback
		if((idi4.channels[i].input_value_cb.period > 0)) {
			if(system_timer_is_time_elapsed_ms(idi4.channels[i].input_value_cb.period_start, idi4.channels[i].input_value_cb.period)) {
				// Period expired

				if(idi4.channels[i].input_value_cb.value_has_to_change) {
					// Enqueue callback if value changed otherwise not
					if(idi4.channels[i].value != idi4.channels[i].input_value_cb.last_value) {
						if(ringbuffer_get_used(&idi4.input_value_cb_rb) < INPUT_VALUE_CB_BUFFER_SIZE) {
							ringbuffer_add(&idi4.input_value_cb_rb, i); // Channel
							ringbuffer_add(&idi4.input_value_cb_rb, (uint8_t)true); // Changed
							ringbuffer_add(&idi4.input_value_cb_rb, (uint8_t)idi4.channels[i].value); // Value
						}
					}
				}
				else {
					// Enqueue callback regardless of change
					if(ringbuffer_get_used(&idi4.input_value_cb_rb) < INPUT_VALUE_CB_BUFFER_SIZE) {
						// Channel
						ringbuffer_add(&idi4.input_value_cb_rb, i);

						// Changed
						if(idi4.channels[i].value != idi4.channels[i].input_value_cb.last_value) {
							ringbuffer_add(&idi4.input_value_cb_rb, (uint8_t)true);
						}
						else {
							ringbuffer_add(&idi4.input_value_cb_rb, (uint8_t)false);
						}

						// Value
						ringbuffer_add(&idi4.input_value_cb_rb, (uint8_t)idi4.channels[i].value);
					}
				}

				// Update last value
				idi4.channels[i].input_value_cb.last_value = idi4.channels[i].value;
				idi4.channels[i].input_value_cb.period_start = system_timer_get_ms();
			}
		}

		// Manage all input value callback
		if(all_ch_in_period_expired) {
			if(idi4.channels[i].value != idi4.all_input_value_cb.last_values[i]) {
				all_ch_in_value_changed = true;
				all_channel_changed |= (1 << i);
			}
			else {
				all_channel_changed &= ~(1 << i);
			}

			if(idi4.channels[i].value) {
				all_channel_values |= (1 << i);
			}
			else {
				all_channel_values &= ~(1 << i);
			}

			// Update last value
			idi4.all_input_value_cb.last_values[i] = idi4.channels[i].value;
		}

		// Manage edge count
		if(system_timer_is_time_elapsed_ms(idi4.channels[i].edge_count.debounce_start, idi4.channels[i].edge_count.debounce)) {
			// Debounce period expired, update edge count

			if(!idi4.channels[i].edge_count.last_value && idi4.channels[i].value) {
				// Rising edge
				idi4.channels[i].edge_count.cnt_edge_rising++;
			}
			else if(idi4.channels[i].edge_count.last_value && !idi4.channels[i].value) {
				// Falling edge
				idi4.channels[i].edge_count.cnt_edge_falling++;
			}

			// Update last value
			idi4.channels[i].edge_count.last_value = idi4.channels[i].value;
			idi4.channels[i].edge_count.debounce_start = system_timer_get_ms();
		}

		// Manage info LEDs
		switch (idi4.info_leds[i].config) {
			case INDUSTRIAL_DIGITAL_IN_4_V2_INFO_LED_CONFIG_OFF:
				idi4.info_leds[i].info_led_flicker_state.config = LED_FLICKER_CONFIG_OFF;
				XMC_GPIO_SetOutputHigh(idi4.info_leds[i].port_base, idi4.info_leds[i].pin); // Info LED off

				break;

			case INDUSTRIAL_DIGITAL_IN_4_V2_INFO_LED_CONFIG_ON:
				idi4.info_leds[i].info_led_flicker_state.config = LED_FLICKER_CONFIG_ON;
				XMC_GPIO_SetOutputLow(idi4.info_leds[i].port_base, idi4.info_leds[i].pin); // Info LED on

				break;

			case INDUSTRIAL_DIGITAL_IN_4_V2_INFO_LED_CONFIG_SHOW_HEARTBEAT:
				idi4.info_leds[i].info_led_flicker_state.config = LED_FLICKER_CONFIG_HEARTBEAT;
				led_flicker_tick(&idi4.info_leds[i].info_led_flicker_state,
				                 system_timer_get_ms(),
				                 idi4.info_leds[i].port_base,
				                 idi4.info_leds[i].pin);

				break;

			case INDUSTRIAL_DIGITAL_IN_4_V2_INFO_LED_CONFIG_SHOW_CHANNEL_STATUS:
				idi4.info_leds[i].info_led_flicker_state.config = LED_FLICKER_CONFIG_OFF;

				if(idi4.channels[i].value) {
					// Input channel has logic high
					XMC_GPIO_SetOutputLow(idi4.info_leds[i].port_base, idi4.info_leds[i].pin); // Info LED on
				}
				else {
					// Input channel has logic low
					XMC_GPIO_SetOutputHigh(idi4.info_leds[i].port_base, idi4.info_leds[i].pin); // Info LED off
				}

				break;

			default:
				break;
		}
	}

	// Manage all input value callback
	if(all_ch_in_period_expired) {
		// Period expired

		if(idi4.all_input_value_cb.value_has_to_change) {
			// Enqueue CB if value changed otherwise not
			if(all_ch_in_value_changed) {
				if(ringbuffer_get_used(&idi4.all_input_value_cb.cb_rb) < ALL_INPUT_VALUE_CB_BUFFER_SIZE) {
					// Changed
					ringbuffer_add(&idi4.all_input_value_cb.cb_rb, all_channel_changed);
					// Value
					ringbuffer_add(&idi4.all_input_value_cb.cb_rb, all_channel_values);
				}
			}
		}
		else {
			// Enqueue CB regardless of change
			if(ringbuffer_get_used(&idi4.all_input_value_cb.cb_rb) < ALL_INPUT_VALUE_CB_BUFFER_SIZE) {
				// Changed
				ringbuffer_add(&idi4.all_input_value_cb.cb_rb, all_channel_changed);
				// Value
				ringbuffer_add(&idi4.all_input_value_cb.cb_rb, all_channel_values);
			}
		}

		idi4.all_input_value_cb.period_start = system_timer_get_ms();
	}
}
