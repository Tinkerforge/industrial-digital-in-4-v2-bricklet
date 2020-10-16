/* industrial-digital-in-4-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
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

#include "configs/config_idi4.h"

#include "bricklib2/logging/logging.h"
#include "bricklib2/hal/system_timer/system_timer.h"

#include "communication.h"

IDI4 idi4;

const uint8_t idi4_channel_pin[IDI4_CHANNEL_NUM] = {
	IDI4_CHANNEL_0_PIN,
	IDI4_CHANNEL_1_PIN,
	IDI4_CHANNEL_2_PIN,
	IDI4_CHANNEL_3_PIN,
};

XMC_GPIO_PORT_t *const idi4_channel_port[IDI4_CHANNEL_NUM] = {
	IDI4_CHANNEL_0_PORT,
	IDI4_CHANNEL_1_PORT,
	IDI4_CHANNEL_2_PORT,
	IDI4_CHANNEL_3_PORT,
};

const uint8_t idi4_channel_led_pin[IDI4_CHANNEL_NUM] = {
	IDI4_CHANNEL_LED_0_PIN,
	IDI4_CHANNEL_LED_1_PIN,
	IDI4_CHANNEL_LED_2_PIN,
	IDI4_CHANNEL_LED_3_PIN
};

XMC_GPIO_PORT_t *const idi4_channel_led_port[IDI4_CHANNEL_NUM] = {
	IDI4_CHANNEL_LED_0_PORT,
	IDI4_CHANNEL_LED_1_PORT,
	IDI4_CHANNEL_LED_2_PORT,
	IDI4_CHANNEL_LED_3_PORT,
};

uint8_t idi4_get_value_bitmask(void) {
	return (idi4.channel_value[0] << 0) | (idi4.channel_value[1] << 1) | (idi4.channel_value[2] << 2) | (idi4.channel_value[3] << 3);
}

void idi4_init(void) {
	memset(&idi4, 0, sizeof(IDI4));

	for(uint8_t i = 0; i < IDI4_CHANNEL_NUM; i++) {
		const XMC_GPIO_CONFIG_t channel_config = {
			.mode                = XMC_GPIO_MODE_INPUT_TRISTATE,
			.input_hysteresis    = XMC_GPIO_INPUT_HYSTERESIS_STANDARD,
		};
		XMC_GPIO_Init(idi4_channel_port[i], idi4_channel_pin[i], &channel_config);

		const XMC_GPIO_CONFIG_t channel_led_config = {
			.mode                = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
			.output_level        = XMC_GPIO_OUTPUT_LEVEL_HIGH
		};
		XMC_GPIO_Init(idi4_channel_led_port[i], idi4_channel_led_pin[i], &channel_led_config);

		// Store initial channel value
		idi4.channel_value[i]       = !XMC_GPIO_GetInput(idi4_channel_port[i], idi4_channel_pin[i]);
		idi4.cb_value_last_value[i] = idi4.channel_value[i];
		idi4.cb_all_last_value     |= (idi4.channel_value[i] << i);

		// Show channel status by default
		idi4.channel_led_flicker_state[i].config = INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS;
	}
}

void idi4_handle_channel_led(const uint8_t channel) {
	if(idi4.channel_led_flicker_state[channel].config == INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_LED_CONFIG_SHOW_HEARTBEAT) {
		led_flicker_tick(&idi4.channel_led_flicker_state[channel], 
		                 system_timer_get_ms(), 
		                 idi4_channel_led_port[channel], 
		                 idi4_channel_led_pin[channel]);
	} else if(idi4.channel_led_flicker_state[channel].config == INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS) {
		if(idi4.channel_value[channel]) {
			XMC_GPIO_SetOutputLow(idi4_channel_led_port[channel], idi4_channel_led_pin[channel]); // Channel LED on
		} else {
			XMC_GPIO_SetOutputHigh(idi4_channel_led_port[channel], idi4_channel_led_pin[channel]); // Channel LED off
		}
	}
}

void idi4_handle_edge_count(const uint8_t channel) {
	if(system_timer_is_time_elapsed_ms(idi4.edge_count[channel].debounce_start, idi4.edge_count[channel].debounce)) {
		// Debounce period expired, update edge count
		if(!idi4.edge_count[channel].last_value && idi4.channel_value[channel]) {
			// Rising edge
			idi4.edge_count[channel].count_edge_rising++;
		} else if(idi4.edge_count[channel].last_value && !idi4.channel_value[channel]) {
			// Falling edge
			idi4.edge_count[channel].count_edge_falling++;
		}

		// Update last value
		idi4.edge_count[channel].last_value = idi4.channel_value[channel];
		idi4.edge_count[channel].debounce_start = system_timer_get_ms();
	}
}

void idi4_tick(void) {
	// Get value for complete port 2
	const uint32_t port2_value = XMC_GPIO_PORT2->IN;
	for(uint8_t i = 0; i < IDI4_CHANNEL_NUM; i++) {
		idi4.channel_value[i] = !((port2_value >> idi4_channel_pin[i]) & 1);

		idi4_handle_edge_count(i);
		idi4_handle_channel_led(i);
	}
}