/* industrial-digital-in-4-v2-bricklet
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
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

#define INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_0 0
#define INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_1 1
#define INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_2 2
#define INDUSTRIAL_DIGITAL_IN_4_V2_CHANNEL_3 3

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
#define FID_SET_VALUE_CALLBACK_CONFIGURATION 2
#define FID_GET_VALUE_CALLBACK_CONFIGURATION 3
#define FID_SET_ALL_VALUE_CALLBACK_CONFIGURATION 4
#define FID_GET_ALL_VALUE_CALLBACK_CONFIGURATION 5
#define FID_GET_EDGE_COUNT 6
#define FID_SET_EDGE_COUNT_CONFIGURATION 7
#define FID_GET_EDGE_COUNT_CONFIGURATION 8
#define FID_SET_CHANNEL_LED_CONFIG 9
#define FID_GET_CHANNEL_LED_CONFIG 10
#define FID_SET_WIEGAND_READER_CONFIG 13
#define FID_GET_WIEGAND_READER_CONFIG 14
#define FID_READ_WIEGAND_DATA_LOW_LEVEL 15
#define FID_SET_WIEGAND_CALLBACK_CONFIG 16
#define FID_GET_WIEGAND_CALLBACK_CONFIG 17
#define FID_GET_WIEGAND_ERROR_COUNT 18

#define FID_CALLBACK_VALUE 11
#define FID_CALLBACK_ALL_VALUE 12
#define FID_CALLBACK_WIEGAND_DATA_LOW_LEVEL 19
#define FID_CALLBACK_WIEGAND_DATA_AVAILABLE 20
#define FID_CALLBACK_WIEGAND_ERROR_COUNT 21

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
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
} __attribute__((__packed__)) GetValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetValueCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetAllValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetAllValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetAllValueCallbackConfiguration_Response;

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
	uint8_t channel;
	uint8_t config;
} __attribute__((__packed__)) SetChannelLEDConfig;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
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
} __attribute__((__packed__)) Value_Callback;

typedef struct {
	TFPMessageHeader header;
	uint8_t changed;
	uint8_t value;
} __attribute__((__packed__)) AllValue_Callback;

typedef struct {
	TFPMessageHeader header;
	bool reader_enabled;
	uint16_t bit_count;
	uint32_t bit_timeout;
} __attribute__((__packed__)) SetWiegandReaderConfig;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetWiegandReaderConfig;

typedef struct {
	TFPMessageHeader header;
	bool reader_enabled;
	uint16_t bit_count;
	uint32_t bit_timeout;
} __attribute__((__packed__)) GetWiegandReaderConfig_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) ReadWiegandDataLowLevel;

typedef struct {
	TFPMessageHeader header;
	uint16_t data_length;
	uint8_t data_data[32];
} __attribute__((__packed__)) ReadWiegandDataLowLevel_Response;

typedef struct {
	TFPMessageHeader header;
	bool data_callback_enabled;
	bool data_available_callback_enabled;
	bool error_count_callback_enabled;
} __attribute__((__packed__)) SetWiegandCallbackConfig;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetWiegandCallbackConfig;

typedef struct {
	TFPMessageHeader header;
	bool data_callback_enabled;
	bool data_available_callback_enabled;
	bool error_count_callback_enabled;
} __attribute__((__packed__)) GetWiegandCallbackConfig_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetWiegandErrorCount;

typedef struct {
	TFPMessageHeader header;
	uint32_t framing_error_count;
	uint32_t overflow_error_count;
} __attribute__((__packed__)) GetWiegandErrorCount_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t data_length;
	uint8_t data_data[32];
} __attribute__((__packed__)) WiegandDataLowLevel_Callback;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) WiegandDataAvailable_Callback;

typedef struct {
	TFPMessageHeader header;
	uint32_t framing_error_count;
	uint32_t overflow_error_count;
} __attribute__((__packed__)) WiegandErrorCount_Callback;


// Function prototypes
BootloaderHandleMessageResponse get_value(const GetValue *data, GetValue_Response *response);
BootloaderHandleMessageResponse set_value_callback_configuration(const SetValueCallbackConfiguration *data);
BootloaderHandleMessageResponse get_value_callback_configuration(const GetValueCallbackConfiguration *data, GetValueCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse set_all_value_callback_configuration(const SetAllValueCallbackConfiguration *data);
BootloaderHandleMessageResponse get_all_value_callback_configuration(const GetAllValueCallbackConfiguration *data, GetAllValueCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse get_edge_count(const GetEdgeCount *data, GetEdgeCount_Response *response);
BootloaderHandleMessageResponse set_edge_count_configuration(const SetEdgeCountConfiguration *data);
BootloaderHandleMessageResponse get_edge_count_configuration(const GetEdgeCountConfiguration *data, GetEdgeCountConfiguration_Response *response);
BootloaderHandleMessageResponse set_channel_led_config(const SetChannelLEDConfig *data);
BootloaderHandleMessageResponse get_channel_led_config(const GetChannelLEDConfig *data, GetChannelLEDConfig_Response *response);
BootloaderHandleMessageResponse set_wiegand_reader_config(const SetWiegandReaderConfig *data);
BootloaderHandleMessageResponse get_wiegand_reader_config(const GetWiegandReaderConfig *data, GetWiegandReaderConfig_Response *response);
BootloaderHandleMessageResponse read_wiegand_data_low_level(const ReadWiegandDataLowLevel *data, ReadWiegandDataLowLevel_Response *response);
BootloaderHandleMessageResponse set_wiegand_callback_config(const SetWiegandCallbackConfig *data);
BootloaderHandleMessageResponse get_wiegand_callback_config(const GetWiegandCallbackConfig *data, GetWiegandCallbackConfig_Response *response);
BootloaderHandleMessageResponse get_wiegand_error_count(const GetWiegandErrorCount *data, GetWiegandErrorCount_Response *response);

// Callbacks
bool handle_value_callback(void);
bool handle_all_value_callback(void);
bool handle_wiegand_data_low_level_callback(void);
bool handle_wiegand_data_available_callback(void);
bool handle_wiegand_error_count_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 5
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_value_callback, \
	handle_all_value_callback, \
	handle_wiegand_data_low_level_callback, \
	handle_wiegand_data_available_callback, \
	handle_wiegand_error_count_callback, \


#endif
