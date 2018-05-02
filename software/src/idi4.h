/* industrial-digital-in-4-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * idi4.h: Implementation of Industrial Digital In V2 Bricklet
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

#ifndef CONFIG_IDI4_H
#define CONFIG_IDI4_H

#include <stdio.h>
#include <stdbool.h>

#include "bricklib2/utility/ringbuffer.h"
#include "bricklib2/utility/led_flicker.h"

#include "xmc_gpio.h"

#define NUMBER_OF_CHANNELS 4

#define INPUT_VALUE_CB_BUFFER_UNIT_SIZE 3
#define INPUT_VALUE_CB_BUFFER_SIZE (256 * INPUT_VALUE_CB_BUFFER_UNIT_SIZE)

#define ALL_INPUT_VALUE_CB_BUFFER_UNIT_SIZE 2
#define ALL_INPUT_VALUE_CB_BUFFER_SIZE (256 * ALL_INPUT_VALUE_CB_BUFFER_UNIT_SIZE)

typedef struct {
	uint8_t pin;
	uint8_t config;
	XMC_GPIO_PORT_t *port_base;
	LEDFlickerState channel_led_flicker_state;
} CHANNEL_LED_CONFIG_t;

typedef struct {
	uint32_t period;
	bool last_value;
	uint32_t period_start;
	bool value_has_to_change;
} CH_IN_VALUE_CB_t;

typedef struct {
	uint32_t period;
	Ringbuffer cb_rb;
	uint32_t period_start;
	bool value_has_to_change;
	bool last_values[NUMBER_OF_CHANNELS];
	uint8_t cb_buffer[ALL_INPUT_VALUE_CB_BUFFER_SIZE];
} ALL_CH_IN_VALUE_CB_t;

typedef struct {
	bool last_value;
	uint8_t debounce;
	uint8_t edge_type;
	uint32_t debounce_start;
	uint32_t cnt_edge_rising;
	uint32_t cnt_edge_falling;
} CH_IN_EDGE_COUNT_t;

typedef struct {
	// Generic channel config
	bool value;
	uint8_t pin;
	XMC_GPIO_PORT_t *port_base;

	// Input channel edge count config
	CH_IN_EDGE_COUNT_t edge_count;
	// Input channel input value callback config
	CH_IN_VALUE_CB_t input_value_cb;
} CH_IN_t;

typedef struct {
	CH_IN_t channels[NUMBER_OF_CHANNELS];
	CHANNEL_LED_CONFIG_t channel_leds[NUMBER_OF_CHANNELS];

	// Input value change callback
	Ringbuffer input_value_cb_rb;
	uint8_t input_value_cb_buffer[INPUT_VALUE_CB_BUFFER_SIZE];

	// All nput value change callback
	ALL_CH_IN_VALUE_CB_t all_input_value_cb;
} IDI4_t;

extern IDI4_t idi4;

void idi4_init(void);
void idi4_tick(void);

#endif
