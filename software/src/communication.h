/* industrial-digital-in-4-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define INDUSTRIAL_DIGITAL_IN_4_V2_EDGE_TYPE_RISING 0
#define INDUSTRIAL_DIGITAL_IN_4_V2_EDGE_TYPE_FALLING 1
#define INDUSTRIAL_DIGITAL_IN_4_V2_EDGE_TYPE_BOTH 2

#define INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_LED_CONFIG_OFF 0
#define INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_LED_CONFIG_ON 1
#define INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_LED_CONFIG_SHOW_HEARTBEAT 2
#define INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS 3

#define INDUSTRIAL_DIGITAL_IN_4_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define INDUSTRIAL_DIGITAL_IN_4_V2_BOOTLOADER_MODE_FIRMWARE 1
#define INDUSTRIAL_DIGITAL_IN_4_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define INDUSTRIAL_DIGITAL_IN_4_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define INDUSTRIAL_DIGITAL_IN_4_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define INDUSTRIAL_DIGITAL_IN_4_V2_BOOTLOADER_STATUS_OK 0
#define INDUSTRIAL_DIGITAL_IN_4_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define INDUSTRIAL_DIGITAL_IN_4_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define INDUSTRIAL_DIGITAL_IN_4_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define INDUSTRIAL_DIGITAL_IN_4_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define INDUSTRIAL_DIGITAL_IN_4_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define INDUSTRIAL_DIGITAL_IN_4_V2_STATUS_LED_CONFIG_OFF 0
#define INDUSTRIAL_DIGITAL_IN_4_V2_STATUS_LED_CONFIG_ON 1
#define INDUSTRIAL_DIGITAL_IN_4_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define INDUSTRIAL_DIGITAL_IN_4_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_GET_VALUE 1
#define FID_GET_SELECTED_VALUE 2
#define FID_SET_INPUT_VALUE_CALLBACK_CONFIGURATION 3
#define FID_GET_INPUT_VALUE_CALLBACK_CONFIGURATION 4
#define FID_SET_ALL_INPUT_VALUE_CALLBACK_CONFIGURATION 5
#define FID_GET_ALL_INPUT_VALUE_CALLBACK_CONFIGURATION 6
#define FID_GET_EDGE_COUNT 7
#define FID_SET_EDGE_COUNT_CONFIGURATION 8
#define FID_GET_EDGE_COUNT_CONFIGURATION 9
#define FID_SET_CHANNEL_LED_CONFIG 10
#define FID_GET_CHANNEL_LED_CONFIG 11

#define FID_CALLBACK_INPUT_VALUE 12
#define FID_CALLBACK_ALL_INPUT_VALUE 13

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetValue;

typedef struct {
	TFPMessageHeader header;
	uint8_t value;
} __attribute__((__packed__)) GetValue_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
} __attribute__((__packed__)) GetSelectedValue;

typedef struct {
	TFPMessageHeader header;
	bool value;
} __attribute__((__packed__)) GetSelectedValue_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetInputValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
} __attribute__((__packed__)) GetInputValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetInputValueCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetAllInputValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetAllInputValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetAllInputValueCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	bool reset_counter;
} __attribute__((__packed__)) GetEdgeCount;

typedef struct {
	TFPMessageHeader header;
	uint32_t count;
} __attribute__((__packed__)) GetEdgeCount_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	uint8_t edge_type;
	uint8_t debounce;
} __attribute__((__packed__)) SetEdgeCountConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
} __attribute__((__packed__)) GetEdgeCountConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t edge_type;
	uint8_t debounce;
} __attribute__((__packed__)) GetEdgeCountConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t led;
	uint8_t config;
} __attribute__((__packed__)) SetChannelLEDConfig;

typedef struct {
	TFPMessageHeader header;
	uint8_t led;
} __attribute__((__packed__)) GetChannelLEDConfig;

typedef struct {
	TFPMessageHeader header;
	uint8_t config;
} __attribute__((__packed__)) GetChannelLEDConfig_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	bool changed;
	bool value;
} __attribute__((__packed__)) InputValue_Callback;

typedef struct {
	TFPMessageHeader header;
	uint8_t changed;
	uint8_t value;
} __attribute__((__packed__)) AllInputValue_Callback;

// Function prototypes
BootloaderHandleMessageResponse get_value(const GetValue *data, GetValue_Response *response);
BootloaderHandleMessageResponse get_selected_value(const GetSelectedValue *data, GetSelectedValue_Response *response);
BootloaderHandleMessageResponse set_input_value_callback_configuration(const SetInputValueCallbackConfiguration *data);
BootloaderHandleMessageResponse get_input_value_callback_configuration(const GetInputValueCallbackConfiguration *data, GetInputValueCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse set_all_input_value_callback_configuration(const SetAllInputValueCallbackConfiguration *data);
BootloaderHandleMessageResponse get_all_input_value_callback_configuration(const GetAllInputValueCallbackConfiguration *data, GetAllInputValueCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse get_edge_count(const GetEdgeCount *data, GetEdgeCount_Response *response);
BootloaderHandleMessageResponse set_edge_count_configuration(const SetEdgeCountConfiguration *data);
BootloaderHandleMessageResponse get_edge_count_configuration(const GetEdgeCountConfiguration *data, GetEdgeCountConfiguration_Response *response);
BootloaderHandleMessageResponse set_channel_led_config(const SetChannelLEDConfig *data);
BootloaderHandleMessageResponse get_channel_led_config(const GetChannelLEDConfig *data, GetChannelLEDConfig_Response *response);

// Callbacks
bool handle_input_value_callback(void);
bool handle_all_input_value_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 2
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_input_value_callback, \
	handle_all_input_value_callback, \

#endif
