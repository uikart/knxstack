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
#include <stdint.h>

uint32_t get_ticks(void);


/* ZEPHYR */
#ifdef CONFIG_OS_ZEPHYR
#include <plat_zephyr.h>
#endif /* CONFIG_OS_ZEPHYR */

/* LINUX */

#ifdef CONFIG_OS_LINUX
#include <plat_linux.h>
#endif /* CONFIG_OS_LINUX */

#endif /* KNX_KNXSTACK_KNX_PLATFORM_H_ */
