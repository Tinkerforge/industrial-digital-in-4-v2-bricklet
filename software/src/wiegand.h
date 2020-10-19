/* industrial-digital-in-4-v2-bricklet
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * wiegand.h: Simple bit banging wiegand receiver
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

#ifndef WIEGAND_H
#define WIEGAND_H

#include <stdint.h>
#include <stdbool.h>

#define WIEGAND_MAX_BIT_COUNT 256

typedef struct {
	bool last_value;
	uint32_t last_value_time;

	uint16_t data_index;
	bool data[1024]; // 256 max bit + 768 bit overflow

	bool out_data_available;
	bool out_data_new;
	uint16_t out_data_length;
	uint8_t out_data_data[32];

	bool out_data_new_cb;
	uint16_t out_data_length_cb;
	uint8_t out_data_data_cb[32];

	uint16_t last_data_index;
	uint32_t last_data_index_time;

	// Config
	bool reader_enabled;
	uint16_t bit_count;
	uint32_t bit_timeout;

	// Error count
	uint32_t framing_error_count;
	uint32_t overflow_error_count;

	// Callbacks
	bool data_callback_enabled;
	bool data_available_callback_enabled;
	bool error_count_callback_enabled;
} Wiegand;

extern Wiegand wiegand;

void wiegand_init(void);
void wiegand_tick(void);
void wiegand_enable(void);
void wiegand_disable(void);

#endif