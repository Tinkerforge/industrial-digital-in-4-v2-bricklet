/* industrial-digital-in-4-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/logging/logging.h"

#include "idi4.h"
#include "wiegand.h"

extern const uint8_t idi4_channel_led_pin[];
extern XMC_GPIO_PORT_t *const idi4_channel_led_port[];

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_VALUE: return get_value(message, response);
		case FID_SET_VALUE_CALLBACK_CONFIGURATION: return set_value_callback_configuration(message);
		case FID_GET_VALUE_CALLBACK_CONFIGURATION: return get_value_callback_configuration(message, response);
		case FID_SET_ALL_VALUE_CALLBACK_CONFIGURATION: return set_all_value_callback_configuration(message);
		case FID_GET_ALL_VALUE_CALLBACK_CONFIGURATION: return get_all_value_callback_configuration(message, response);
		case FID_GET_EDGE_COUNT: return get_edge_count(message, response);
		case FID_SET_EDGE_COUNT_CONFIGURATION: return set_edge_count_configuration(message);
		case FID_GET_EDGE_COUNT_CONFIGURATION: return get_edge_count_configuration(message, response);
		case FID_SET_CHANNEL_LED_CONFIG: return set_channel_led_config(message);
		case FID_GET_CHANNEL_LED_CONFIG: return get_channel_led_config(message, response);
		case FID_SET_WIEGAND_READER_CONFIG: return set_wiegand_reader_config(message);
		case FID_GET_WIEGAND_READER_CONFIG: return get_wiegand_reader_config(message, response);
		case FID_READ_WIEGAND_DATA_LOW_LEVEL: return read_wiegand_data_low_level(message, response);
		case FID_SET_WIEGAND_CALLBACK_CONFIG: return set_wiegand_callback_config(message);
		case FID_GET_WIEGAND_CALLBACK_CONFIG: return get_wiegand_callback_config(message, response);
		case FID_GET_WIEGAND_ERROR_COUNT: return get_wiegand_error_count(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse get_value(const GetValue *data, GetValue_Response *response) {
	response->header.length = sizeof(GetValue_Response);
	response->value         = idi4_get_value_bitmask();

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_value_callback_configuration(const SetValueCallbackConfiguration *data) {
	if(data->channel >= IDI4_CHANNEL_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}	

	idi4.cb_value_period[data->channel]        = data->period;
	idi4.cb_value_has_to_change[data->channel] = data->value_has_to_change;
	idi4.cb_value_last_value[data->channel]    = idi4.channel_value[data->channel];
	idi4.cb_value_last_time[data->channel]     = 0;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_value_callback_configuration(const GetValueCallbackConfiguration *data, GetValueCallbackConfiguration_Response *response) {
	if(data->channel >= IDI4_CHANNEL_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length       = sizeof(GetValueCallbackConfiguration_Response);
	response->period              = idi4.cb_value_period[data->channel];
	response->value_has_to_change = idi4.cb_value_has_to_change[data->channel];

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_all_value_callback_configuration(const SetAllValueCallbackConfiguration *data) {
	idi4.cb_all_period        = data->period;
	idi4.cb_all_has_to_change = data->value_has_to_change;
	idi4.cb_all_last_value    = idi4_get_value_bitmask();
	idi4.cb_all_last_time     = 0;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_all_value_callback_configuration(const GetAllValueCallbackConfiguration *data, GetAllValueCallbackConfiguration_Response *response) {
	response->header.length       = sizeof(GetValueCallbackConfiguration_Response);
	response->period              = idi4.cb_all_period;
	response->value_has_to_change = idi4.cb_all_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_edge_count(const GetEdgeCount *data, GetEdgeCount_Response *response) {
	if(data->channel >= IDI4_CHANNEL_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}	

	response->header.length = sizeof(GetEdgeCount_Response);

	if(idi4.edge_count[data->channel].edge_type == INDUSTRIAL_DIGITAL_IN_4_V2_EDGE_TYPE_RISING) {
		response->count = idi4.edge_count[data->channel].count_edge_rising;
	} else if(idi4.edge_count[data->channel].edge_type == INDUSTRIAL_DIGITAL_IN_4_V2_EDGE_TYPE_FALLING) {
		response->count = idi4.edge_count[data->channel].count_edge_falling;
	} else if(idi4.edge_count[data->channel].edge_type == INDUSTRIAL_DIGITAL_IN_4_V2_EDGE_TYPE_BOTH) {
		response->count = idi4.edge_count[data->channel].count_edge_rising + idi4.edge_count[data->channel].count_edge_falling;
	}

	if(data->reset_counter) {
		idi4.edge_count[data->channel].count_edge_rising  = 0;
		idi4.edge_count[data->channel].count_edge_falling = 0;
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_edge_count_configuration(const SetEdgeCountConfiguration *data) {
	if(data->channel >= IDI4_CHANNEL_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}	

	idi4.edge_count[data->channel].count_edge_rising  = 0;
	idi4.edge_count[data->channel].count_edge_falling = 0;
	idi4.edge_count[data->channel].debounce           = data->debounce;
	idi4.edge_count[data->channel].edge_type          = data->edge_type;
	idi4.edge_count[data->channel].last_value         = idi4.channel_value[data->channel];
	idi4.edge_count[data->channel].debounce_start     = system_timer_get_ms();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_edge_count_configuration(const GetEdgeCountConfiguration *data, GetEdgeCountConfiguration_Response *response) {
	if(data->channel >= IDI4_CHANNEL_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetEdgeCountConfiguration_Response);
	response->debounce      = idi4.edge_count[data->channel].debounce;
	response->edge_type     = idi4.edge_count[data->channel].edge_type;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_channel_led_config(const SetChannelLEDConfig *data) {
	if(data->channel >= IDI4_CHANNEL_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(data->config > INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	idi4.channel_led_flicker_state[data->channel].config = data->config;
	if(data->config == INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_LED_CONFIG_OFF) {
		XMC_GPIO_SetOutputHigh(idi4_channel_led_port[data->channel], idi4_channel_led_pin[data->channel]);
	} else if(data->config == INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_LED_CONFIG_ON) {
		XMC_GPIO_SetOutputLow(idi4_channel_led_port[data->channel], idi4_channel_led_pin[data->channel]);
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_channel_led_config(const GetChannelLEDConfig *data, GetChannelLEDConfig_Response *response) {
	if(data->channel >= IDI4_CHANNEL_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetChannelLEDConfig_Response);
	response->config        = idi4.channel_led_flicker_state[data->channel].config;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_wiegand_reader_config(const SetWiegandReaderConfig *data) {
	if(data->bit_count > WIEGAND_MAX_BIT_COUNT) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if((data->bit_timeout == 0) && (data->bit_count == 0)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	wiegand.bit_count   = data->bit_count;
	wiegand.bit_timeout = data->bit_timeout;

	if(data->reader_enabled) {
		wiegand_enable();
	} else {
		wiegand_disable();
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_wiegand_reader_config(const GetWiegandReaderConfig *data, GetWiegandReaderConfig_Response *response) {
	response->header.length  = sizeof(GetWiegandReaderConfig_Response);
	response->bit_count      = wiegand.bit_count;
	response->bit_timeout    = wiegand.bit_timeout;
	response->reader_enabled = wiegand.reader_enabled;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse read_wiegand_data_low_level(const ReadWiegandDataLowLevel *data, ReadWiegandDataLowLevel_Response *response) {
	response->header.length = sizeof(ReadWiegandDataLowLevel_Response);
	if(wiegand.out_data_new) {
		response->data_length = wiegand.out_data_length;
		memcpy(response->data_data, wiegand.out_data_data, 32);
		wiegand.out_data_new = false;
	} else {
		response->data_length = 0;
		memset(response->data_data, 0, 32);
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_wiegand_callback_config(const SetWiegandCallbackConfig *data) {
	wiegand.data_available_callback_enabled = data->data_available_callback_enabled;
	wiegand.data_callback_enabled           = data->data_callback_enabled;
	wiegand.error_count_callback_enabled    = data->error_count_callback_enabled;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_wiegand_callback_config(const GetWiegandCallbackConfig *data, GetWiegandCallbackConfig_Response *response) {
	response->header.length                   = sizeof(GetWiegandCallbackConfig_Response);
	response->data_available_callback_enabled = wiegand.data_available_callback_enabled;
	response->data_callback_enabled           = wiegand.data_callback_enabled;
	response->error_count_callback_enabled    = wiegand.error_count_callback_enabled;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_wiegand_error_count(const GetWiegandErrorCount *data, GetWiegandErrorCount_Response *response) {
	response->header.length        = sizeof(GetWiegandErrorCount_Response);
	response->framing_error_count  = wiegand.framing_error_count;
	response->overflow_error_count = wiegand.overflow_error_count;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_value_callback_channel(const uint8_t channel) {
	static bool is_buffered[IDI4_CHANNEL_NUM] = {false, false, false, false};
	static Value_Callback cb[IDI4_CHANNEL_NUM];

	if(!is_buffered[channel]) {
		if((idi4.cb_value_period[channel] == 0) || !system_timer_is_time_elapsed_ms(idi4.cb_value_last_time[channel], idi4.cb_value_period[channel])) {
			return false;
		}

		const bool changed = idi4.cb_value_last_value[channel] != idi4.channel_value[channel];
		if(idi4.cb_value_has_to_change[channel] && !changed) {
			return false;
		}
			
		tfp_make_default_header(&cb[channel].header, bootloader_get_uid(), sizeof(Value_Callback), FID_CALLBACK_VALUE);
		cb[channel].channel = channel;
		cb[channel].changed = changed;
		cb[channel].value   = idi4.channel_value[channel];
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb[channel], sizeof(Value_Callback));
		is_buffered[channel] = false;

		return true;
	} else {
		is_buffered[channel] = true;
	}

	return false;
}



bool handle_value_callback(void) {
	static uint8_t channel = 0;

	// Go through all channels round robin until one of the channels has something to send
	for(uint8_t i = 0; i < IDI4_CHANNEL_NUM; i++) {
		bool ret = handle_value_callback_channel(channel);
		channel = (channel+1) % 4;
		if(ret) {
			return true;
		}
	}

	return false;
}

bool handle_all_value_callback(void) {
	static bool is_buffered = false;
	static AllValue_Callback cb;

	if(!is_buffered) {
		if((idi4.cb_all_period == 0) || !system_timer_is_time_elapsed_ms(idi4.cb_all_last_time, idi4.cb_all_period)) {
			return false;
		}

		const uint8_t value   = idi4_get_value_bitmask();
		const uint8_t changed = idi4.cb_all_last_value ^ value;
		if(idi4.cb_all_has_to_change && (changed == 0)) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(Value_Callback), FID_CALLBACK_VALUE);
		cb.changed = changed;
		cb.value   = value;
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(Value_Callback));
		is_buffered = false;

		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_wiegand_data_low_level_callback(void) {
	static bool is_buffered = false;
	static WiegandDataLowLevel_Callback cb;

	if(!is_buffered) {
		if(!wiegand.out_data_new_cb) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(WiegandDataLowLevel_Callback), FID_CALLBACK_WIEGAND_DATA_LOW_LEVEL);
		cb.data_length = wiegand.out_data_length_cb;
		memcpy(cb.data_data, wiegand.out_data_data_cb, 32);

		wiegand.out_data_new_cb = false;
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(WiegandDataLowLevel_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_wiegand_data_available_callback(void) {
	static bool is_buffered = false;
	static WiegandDataAvailable_Callback cb;

	if(!is_buffered) {
		if(!wiegand.out_data_available) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(WiegandDataAvailable_Callback), FID_CALLBACK_WIEGAND_DATA_AVAILABLE);
		wiegand.out_data_available = false;
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(WiegandDataAvailable_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_wiegand_error_count_callback(void) {
	static bool is_buffered = false;
	static WiegandErrorCount_Callback cb;

	static uint32_t last_framing_error_count  = 0;
	static uint32_t last_overflow_error_count = 0;

	if(!is_buffered) {
		if(!wiegand.error_count_callback_enabled) {
			return false;
		}

		if((last_framing_error_count == wiegand.framing_error_count) && (last_overflow_error_count == wiegand.overflow_error_count)) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(WiegandErrorCount_Callback), FID_CALLBACK_WIEGAND_ERROR_COUNT);
		cb.framing_error_count    = wiegand.framing_error_count;
		cb.overflow_error_count   = wiegand.overflow_error_count;

		last_framing_error_count  = wiegand.framing_error_count;
		last_overflow_error_count = wiegand.overflow_error_count;
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(WiegandErrorCount_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}


void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
