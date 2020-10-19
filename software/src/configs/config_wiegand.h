/* industrial-digital-in-4-v2-bricklet
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_wiegand.h: Wiegand receiver input IRQ config
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

#ifndef CONFIG_WIEGAND_H
#define CONFIG_WIEGAND_H

#define WIEGAND_DATA0_ETL_INA         XMC_ERU_ETL_INPUT_A0
#define WIEGAND_DATA0_ETL_INB         XMC_ERU_ETL_INPUT_B1
#define WIEGAND_DATA0_ETL_SOURCE      XMC_ERU_ETL_SOURCE_B
#define WIEGAND_DATA0_ETL_TRIGGER     XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL1
#define WIEGAND_DATA0_ETL_CHANNEL     0
#define WIEGAND_DATA0_OGU_CHANNEL     1

#define WIEGAND_DATA0_IRQ_N           4
#define WIEGAND_DATA0_IRQ_PRIO        1


#define WIEGAND_DATA1_ETL_INA         XMC_ERU_ETL_INPUT_A1
#define WIEGAND_DATA1_ETL_INB         XMC_ERU_ETL_INPUT_B0
#define WIEGAND_DATA1_ETL_SOURCE      XMC_ERU_ETL_SOURCE_A
#define WIEGAND_DATA1_ETL_TRIGGER     XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL0
#define WIEGAND_DATA1_ETL_CHANNEL     2
#define WIEGAND_DATA1_OGU_CHANNEL     0

#define WIEGAND_DATA1_IRQ_N           3
#define WIEGAND_DATA1_IRQ_PRIO        1


#endif