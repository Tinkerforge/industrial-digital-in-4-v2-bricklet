/* industrial-digital-in-4-v2-bricklet
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_idi4.h: Industrial Digital In 4 V2 Bricklet driver config
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

#include "xmc_gpio.h"

#define IDI4_CHANNEL_0_PIN      2
#define IDI4_CHANNEL_0_PORT     XMC_GPIO_PORT2
#define IDI4_CHANNEL_1_PIN      6
#define IDI4_CHANNEL_1_PORT     XMC_GPIO_PORT2
#define IDI4_CHANNEL_2_PIN      7
#define IDI4_CHANNEL_2_PORT     XMC_GPIO_PORT2
#define IDI4_CHANNEL_3_PIN      9
#define IDI4_CHANNEL_3_PORT     XMC_GPIO_PORT2

#define IDI4_CHANNEL_LED_0_PIN  5
#define IDI4_CHANNEL_LED_0_PORT XMC_GPIO_PORT0
#define IDI4_CHANNEL_LED_1_PIN  0
#define IDI4_CHANNEL_LED_1_PORT XMC_GPIO_PORT0
#define IDI4_CHANNEL_LED_2_PIN  0
#define IDI4_CHANNEL_LED_2_PORT XMC_GPIO_PORT1
#define IDI4_CHANNEL_LED_3_PIN  1
#define IDI4_CHANNEL_LED_3_PORT XMC_GPIO_PORT1

#endif