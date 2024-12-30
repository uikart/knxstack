/**
 * \brief |
 *
 * \filename plat_linux.c
 * \author undead
 * \date 10 lis 2024
 *
 * \license This Source Code Form is subject to the terms of the Mozilla Public
 * 			License, v. 2.0. If a copy of the MPL was not distributed with this
 * 			file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <knx_config.h>
#include <knx_log.h>
#include <knxstack.h>
#include <knx.h>
#define KNX_DEBUG_FILE KNX_DEBUG_PLATFORM

/* this is version for the zephyr */
uint32_t get_ticks(void)
{
	return k_uptime_get_32();
}

int _knx_read_byte(const knx_device_t *device)
{
	int byte = knx_read_byte((const struct device *)device);
//	KNX_LOG_DBG("Read byte: [0x%02x]\n", byte);
	return byte;
}

size_t _knx_wrtie_data(const knx_device_t *device, uint8_t *buf, size_t count)
{
	size_t i;

	for (i = 0; i < count; i++) {
		if (knx_write_byte((const struct device *)device, buf[i]) < 0 )
			break;
	}
	return i;
}

int _knx_flush_data(const knx_device_t *device)
{
	return knx_reset_bus(device);;
}
