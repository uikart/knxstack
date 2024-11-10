/**
 * \brief file define the Platform that can be run the stack
 *
 * \filename knx_platform.h
 * \author undead
 * \date 10 lis 2024
 *
 * \license This Source Code Form is subject to the terms of the Mozilla Public
 * 			License, v. 2.0. If a copy of the MPL was not distributed with this
 * 			file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 */

#ifndef KNX_KNXSTACK_KNX_PLATFORM_H_
#define KNX_KNXSTACK_KNX_PLATFORM_H_
#include <knx_config.h>

/* ZEPHYR */
#ifdef CONFIG_OS_ZEPHYR
#include <zephyr/device.h>

typedef  struct device* knx_device_t;
#endif /* CONFIG_OS_ZEPHYR */

/* LINUX */

#include <plat_linux.h>

#ifdef CONFIG_OS_LINUX
typedef void *knx_device_t;
#endif /* CONFIG_OS_LINUX */

#endif /* KNX_KNXSTACK_KNX_PLATFORM_H_ */
