/**
 * \brief |
 *
 * \filename plat_linux.h
 * \author undead
 * \date 29 Dec 2024
 *
 * \license This Source Code Form is subject to the terms of the Mozilla Public
 * 			License, v. 2.0. If a copy of the MPL was not distributed with this
 * 			file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 */

#ifndef KNX_KNXSTACK_INCLUDE_PLAT_ZEPHYR_H_
#define KNX_KNXSTACK_INCLUDE_PLAT_ZEPHYR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <knx_platform.h>
#include <knxstack.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>

typedef  struct device knx_device_t;

int _knx_read_byte(const knx_device_t *device);
size_t _knx_wrtie_data(const knx_device_t *device, uint8_t *buf, size_t count);
int _knx_flush_data(const knx_device_t *device);


#endif /* KNX_KNXSTACK_INCLUDE_PLAT_ZEPHYR_H_ */
