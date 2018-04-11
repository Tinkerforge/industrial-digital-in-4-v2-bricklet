/* industrial-digital-in-4-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
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

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_VALUE: return get_value(message, response);
		case FID_GET_SELECTED_VALUE: return get_selected_value(message, response);
		case FID_SET_INPUT_VALUE_CALLBACK_CONFIGURATION: return set_input_value_callback_configuration(message);
		case FID_GET_INPUT_VALUE_CALLBACK_CONFIGURATION: return get_input_value_callback_configuration(message, response);
		case FID_SET_ALL_INPUT_VALUE_CALLBACK_CONFIGURATION: return set_all_input_value_callback_configuration(message);
		case FID_GET_ALL_INPUT_VALUE_CALLBACK_CONFIGURATION: return get_all_input_value_callback_configuration(message, response);
		case FID_GET_EDGE_COUNT: return get_edge_count(message, response);
		case FID_SET_EDGE_COUNT_CONFIGURATION: return set_edge_count_configuration(message);
		case FID_GET_EDGE_COUNT_CONFIGURATION: return get_edge_count_configuration(message, response);
		case FID_SET_INFO_LED_CONFIG: return set_info_led_config(message);
		case FID_GET_INFO_LED_CONFIG: return get_info_led_config(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse get_value(const GetValue *data, GetValue_Response *response) {
	logd("[+] IDI4-V2: get_value()\n\r");

	uint8_t bit_encoded_values = 0;
	response->header.length = sizeof(GetValue_Response);

	for(uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
		if(idi4.channels[i].value) {
			bit_encoded_values |= (1 << i);
		}
		else {
			bit_encoded_values &= ~(1 << i);
		}
	}

	response->value = bit_encoded_values;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_selected_value(const GetSelectedValue *data, GetSelectedValue_Response *response) {
	logd("[+] IDI4-V2: get_selected_value()\n\r");

	response->header.length = sizeof(GetSelectedValue_Response);

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->value = idi4.channels[data->channel].value;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_input_value_callback_configuration(const SetInputValueCallbackConfiguration *data) {
	logd("[+] IDI4-V2: set_input_value_callback_configuration()\n\r");

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	idi4.channels[data->channel].input_value_cb.period_start = 0;
	idi4.channels[data->channel].input_value_cb.period = data->period;
	idi4.channels[data->channel].input_value_cb.value_has_to_change = data->value_has_to_change;

	if(idi4.channels[data->channel].input_value_cb.period > 0) {
		idi4.channels[data->channel].input_value_cb.last_value = \
			!((bool)XMC_GPIO_GetInput(idi4.channels[data->channel].port_base,
		                              idi4.channels[data->channel].pin));
		idi4.channels[data->channel].input_value_cb.period_start = system_timer_get_ms();
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_input_value_callback_configuration(const GetInputValueCallbackConfiguration *data, GetInputValueCallbackConfiguration_Response *response) {
	logd("[+] IDI4-V2: get_input_value_callback_configuration()\n\r");

	response->header.length = sizeof(GetInputValueCallbackConfiguration_Response);

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->period = idi4.channels[data->channel].input_value_cb.period;
	response->value_has_to_change = idi4.channels[data->channel].input_value_cb.value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_all_input_value_callback_configuration(const SetAllInputValueCallbackConfiguration *data) {
	logd("[+] IDI4-V2: set_all_input_value_callback_configuration()\n\r");

	idi4.all_input_value_cb.period_start = 0;
	idi4.all_input_value_cb.period = data->period;
	idi4.all_input_value_cb.value_has_to_change = data->value_has_to_change;

	if(idi4.all_input_value_cb.period > 0) {		
		// Store current channel states
		for(uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
			idi4.all_input_value_cb.last_values[i] = \
				!((bool)XMC_GPIO_GetInput(idi4.channels[i].port_base, idi4.channels[i].pin));
		}

		idi4.all_input_value_cb.period_start = system_timer_get_ms();
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_all_input_value_callback_configuration(const GetAllInputValueCallbackConfiguration *data, GetAllInputValueCallbackConfiguration_Response *response) {
	logd("[+] IDI4-V2: get_all_input_value_callback_configuration()\n\r");

	response->header.length = sizeof(GetInputValueCallbackConfiguration_Response);
	response->period = idi4.all_input_value_cb.period;
	response->value_has_to_change = idi4.all_input_value_cb.value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_edge_count(const GetEdgeCount *data, GetEdgeCount_Response *response) {
	logd("[+] IDI4-V2: get_edge_count()\n\r");

	response->header.length = sizeof(GetEdgeCount_Response);

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(idi4.channels[data->channel].edge_count.edge_type == INDUSTRIAL_DIGITAL_IN_4_V2_EDGE_TYPE_RISING) {
		response->count = idi4.channels[data->channel].edge_count.cnt_edge_rising;
	}
	else if(idi4.channels[data->channel].edge_count.edge_type == INDUSTRIAL_DIGITAL_IN_4_V2_EDGE_TYPE_FALLING) {
		response->count = idi4.channels[data->channel].edge_count.cnt_edge_falling;
	}
	else if(idi4.channels[data->channel].edge_count.edge_type == INDUSTRIAL_DIGITAL_IN_4_V2_EDGE_TYPE_BOTH) {
		response->count = \
			idi4.channels[data->channel].edge_count.cnt_edge_rising + \
			idi4.channels[data->channel].edge_count.cnt_edge_falling;
	}

	if(data->reset_counter) {
		idi4.channels[data->channel].edge_count.cnt_edge_rising = 0;
		idi4.channels[data->channel].edge_count.cnt_edge_falling = 0;
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_edge_count_configuration(const SetEdgeCountConfiguration *data) {
	logd("[+] IDI4-V2: set_edge_count_configuration()\n\r");

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	idi4.channels[data->channel].edge_count.cnt_edge_rising = 0;
	idi4.channels[data->channel].edge_count.cnt_edge_falling = 0;
	idi4.channels[data->channel].edge_count.debounce = data->debounce;
	idi4.channels[data->channel].edge_count.edge_type = data->edge_type;
	idi4.channels[data->channel].edge_count.last_value = \
		!((bool)XMC_GPIO_GetInput(idi4.channels[data->channel].port_base,
		                          idi4.channels[data->channel].pin));
	idi4.channels[data->channel].edge_count.debounce_start = system_timer_get_ms();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_edge_count_configuration(const GetEdgeCountConfiguration *data, GetEdgeCountConfiguration_Response *response) {
	logd("[+] IDI4-V2: get_edge_count_configuration()\n\r");

	response->header.length = sizeof(GetEdgeCountConfiguration_Response);

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->debounce = idi4.channels[data->channel].edge_count.debounce;
	response->edge_type = idi4.channels[data->channel].edge_count.edge_type;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_info_led_config(const SetInfoLEDConfig *data) {
	logd("[+] IDI4-V2: set_edge_count_configuration()\n\r");

	if(data->led > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	idi4.info_leds[data->led].config = data->config;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_info_led_config(const GetInfoLEDConfig *data, GetInfoLEDConfig_Response *response) {
	logd("[+] IDI4-V2: get_info_led_config()\n\r");

	response->header.length = sizeof(GetInfoLEDConfig_Response);

	if(data->led > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->config = idi4.info_leds[data->led].config;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_input_value_callback(void) {
	static bool is_buffered = false;
	static InputValue_Callback cb;

	if(!is_buffered) {
		if(ringbuffer_is_empty(&idi4.input_value_cb_rb)) {
			// Nothing to send
			return false;
		}

		tfp_make_default_header(&cb.header,
		                        bootloader_get_uid(),
		                        sizeof(InputValue_Callback),
		                        FID_CALLBACK_INPUT_VALUE);

		ringbuffer_get(&idi4.input_value_cb_rb, &cb.channel);
		ringbuffer_get(&idi4.input_value_cb_rb, (uint8_t *)&cb.changed);
		ringbuffer_get(&idi4.input_value_cb_rb, (uint8_t *)&cb.value);
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status,
		                                       (uint8_t*)&cb,
		                                       sizeof(InputValue_Callback));
		is_buffered = false;

		return true;
	}
	else {
		is_buffered = true;
	}

	return false;
}

bool handle_all_input_value_callback(void) {
	static bool is_buffered = false;
	static AllInputValue_Callback cb;

	if(!is_buffered) {
		if(ringbuffer_is_empty(&idi4.all_input_value_cb.cb_rb)) {
			// Nothing to send
			return false;
		}

		tfp_make_default_header(&cb.header,
		                        bootloader_get_uid(),
		                        sizeof(AllInputValue_Callback),
		                        FID_CALLBACK_ALL_INPUT_VALUE);

		ringbuffer_get(&idi4.all_input_value_cb.cb_rb, &cb.changed);
		ringbuffer_get(&idi4.all_input_value_cb.cb_rb, &cb.value);
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status,
		                                       (uint8_t*)&cb,
		                                       sizeof(AllInputValue_Callback));

		is_buffered = false;

		return true;
	}
	else {
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
