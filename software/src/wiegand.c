/* industrial-digital-in-4-v2-bricklet
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * wiegand.c: Simple bit banging wiegand receiver
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

#include "wiegand.h"

#include "bricklib2/logging/logging.h"

#include "xmc_gpio.h"
#include "xmc_eru.h"

#include "idi4.h"
#include "configs/config_wiegand.h"

Wiegand wiegand;

extern const uint8_t idi4_channel_pin[];
extern XMC_GPIO_PORT_t *const idi4_channel_port[];

#define wiegand_irq_data0 IRQ_Hdlr_4
#define wiegand_irq_data1 IRQ_Hdlr_3



void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) wiegand_irq_data0(void) {
	wiegand.data[wiegand.data_index++] = 0;
}

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) wiegand_irq_data1(void) {
	wiegand.data[wiegand.data_index++] = 1;
}

void wiegand_init(void) {
	memset(&wiegand, 0, sizeof(Wiegand));

	wiegand.bit_count   = 26;
	wiegand.bit_timeout = 50;

	const XMC_ERU_ETL_CONFIG_t sync_etl_config_data0 = {
		.input_a                = WIEGAND_DATA0_ETL_INA,
		.input_b                = WIEGAND_DATA0_ETL_INB,
		.enable_output_trigger  = 1,
		.edge_detection         = XMC_ERU_ETL_EDGE_DETECTION_FALLING,
		.output_trigger_channel = WIEGAND_DATA0_ETL_TRIGGER,
		.source                 = WIEGAND_DATA0_ETL_SOURCE
	};
	XMC_ERU_ETL_Init(XMC_ERU0, WIEGAND_DATA0_ETL_CHANNEL, &sync_etl_config_data0);
	XMC_ERU_OGU_SetServiceRequestMode(XMC_ERU0, WIEGAND_DATA0_OGU_CHANNEL, XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER);
	NVIC_SetPriority(WIEGAND_DATA0_IRQ_N, WIEGAND_DATA0_IRQ_PRIO);

	const XMC_ERU_ETL_CONFIG_t sync_etl_config_data1 = {
		.input_a                = WIEGAND_DATA1_ETL_INA,
		.input_b                = WIEGAND_DATA1_ETL_INB,
		.enable_output_trigger  = 1,
		.edge_detection         = XMC_ERU_ETL_EDGE_DETECTION_FALLING,
		.output_trigger_channel = WIEGAND_DATA1_ETL_TRIGGER,
		.source                 = WIEGAND_DATA1_ETL_SOURCE
	};
	XMC_ERU_ETL_Init(XMC_ERU0, WIEGAND_DATA1_ETL_CHANNEL, &sync_etl_config_data1);
	XMC_ERU_OGU_SetServiceRequestMode(XMC_ERU0, WIEGAND_DATA1_OGU_CHANNEL, XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER);
	NVIC_SetPriority(WIEGAND_DATA1_IRQ_N, WIEGAND_DATA1_IRQ_PRIO);
}

void wiegand_enable(void) {
	wiegand.data_index           = 0;
	wiegand.last_data_index      = 0;
	wiegand.last_data_index_time = 0;
	NVIC_ClearPendingIRQ(WIEGAND_DATA0_IRQ_PRIO);
	NVIC_EnableIRQ(WIEGAND_DATA0_IRQ_N);
	NVIC_ClearPendingIRQ(WIEGAND_DATA1_IRQ_PRIO);
	NVIC_EnableIRQ(WIEGAND_DATA1_IRQ_N);
	wiegand.reader_enabled = true;
}

void wiegand_disable(void) {
	NVIC_ClearPendingIRQ(WIEGAND_DATA0_IRQ_PRIO);
	NVIC_DisableIRQ(WIEGAND_DATA0_IRQ_N);
	NVIC_ClearPendingIRQ(WIEGAND_DATA1_IRQ_PRIO);
	NVIC_DisableIRQ(WIEGAND_DATA1_IRQ_N);
	wiegand.reader_enabled = false;

	wiegand.data_index           = 0;
	wiegand.last_data_index      = 0;
	wiegand.last_data_index_time = 0;
}

void wiegand_reset(void) {
	wiegand_disable();
	wiegand_enable();
}

bool wiegand_check_frame_done(void) {
	if(wiegand.last_data_index == 0) {
		return false;
	}

	if(wiegand.bit_count == 0) {
		return system_timer_is_time_elapsed_ms(wiegand.last_data_index_time, wiegand.bit_timeout);
	} else if(wiegand.bit_timeout == 0) {
		return wiegand.bit_count <= wiegand.data_index;
	}

	return system_timer_is_time_elapsed_ms(wiegand.last_data_index_time, wiegand.bit_timeout) || (wiegand.bit_count <= wiegand.data_index);
}

void wiegand_tick(void) {
	if(wiegand.reader_enabled) {
		if((wiegand.bit_count != 0) &&(wiegand.data_index > wiegand.bit_count)) {
			wiegand.framing_error_count++;
			wiegand_reset();
		}

		if(wiegand.data_index > WIEGAND_MAX_BIT_COUNT) {
			wiegand.overflow_error_count++;
			wiegand_reset();
		}

		if(wiegand.data_index != 0) {
			if(wiegand_check_frame_done()) {
				if((wiegand.bit_count != 0) && (wiegand.data_index != wiegand.bit_count)) {
					wiegand.framing_error_count++;
				} else {
					wiegand.out_data_length = wiegand.data_index;
					memset(wiegand.out_data_data, 0, 32);
					for(uint16_t i = 0; i < wiegand.data_index; i++) {
						wiegand.out_data_data[i/8] |= (wiegand.data[i] << (i % 8));
					}
					wiegand.out_data_new = true;

					if(wiegand.data_available_callback_enabled) {
						wiegand.out_data_available = true;
					}

					if(wiegand.data_callback_enabled) {
						wiegand.out_data_length_cb = wiegand.data_index;
						memset(wiegand.out_data_data_cb, 0, 32);
						for(uint16_t i = 0; i < wiegand.data_index; i++) {
							wiegand.out_data_data_cb[i/8] |= (wiegand.data[i] << (i % 8));
						}
						wiegand.out_data_new_cb = true;
					}
				}

				wiegand_reset();
			} else {
				if(wiegand.last_data_index != wiegand.data_index) {
					wiegand.last_data_index_time = system_timer_get_ms();
					wiegand.last_data_index      = wiegand.data_index;
				}
			}
		}
	}
}