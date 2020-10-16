/* industrial-digital-in-4-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
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

#ifndef IDI4_H
#define IDI4_H

#include <stdio.h>
#include <stdbool.h>

#include "bricklib2/utility/ringbuffer.h"
#include "bricklib2/utility/led_flicker.h"

#include "xmc_gpio.h"

#define IDI4_CHANNEL_NUM 4

typedef struct {
	bool     last_value;
	uint8_t  debounce;
	uint8_t  edge_type;
	uint32_t debounce_start;
	uint32_t count_edge_rising;
	uint32_t count_edge_falling;
} IDI4EdgeCount;

typedef struct {
	bool            channel_value[IDI4_CHANNEL_NUM];
	LEDFlickerState channel_led_flicker_state[IDI4_CHANNEL_NUM];
	IDI4EdgeCount   edge_count[IDI4_CHANNEL_NUM];

	uint32_t cb_value_period[IDI4_CHANNEL_NUM];
	bool     cb_value_has_to_change[IDI4_CHANNEL_NUM];
	uint32_t cb_value_last_time[IDI4_CHANNEL_NUM];
	bool     cb_value_last_value[IDI4_CHANNEL_NUM];

	uint32_t cb_all_period;
	bool     cb_all_has_to_change;
	uint32_t cb_all_last_time;
	uint8_t  cb_all_last_value;
} IDI4;

extern IDI4 idi4;


uint8_t idi4_get_value_bitmask(void);
void idi4_init(void);
void idi4_tick(void);

#endif
