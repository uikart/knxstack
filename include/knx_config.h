/**
 * \brief |
 *
 * \filename knx_config.h
 * \author undead
 * \date 10 lis 2024
 *
 * \license This Source Code Form is subject to the terms of the Mozilla Public
 * 			License, v. 2.0. If a copy of the MPL was not distributed with this
 * 			file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 */

#ifndef KNX_KNXSTACK_INCLUDE_KNX_CONFIG_H_
#define KNX_KNXSTACK_INCLUDE_KNX_CONFIG_H_

#define CONFIG_OS_ZEPHYR 1
//#define CONFIG_OS_LINUX 1

#define CONFIG_PLATFORM_STM32_STKNX 1

#define CONFIG_KNX_DEBUG (KNX_DEBUG_PKG_DUMP | KNX_DEBUG_L1 | KNX_DEBUG_PLATFORM)
#define CONFIG_KNX_DEBUG_LEVEL KNX_LOG_LEVEL_DBG

//#define CONFIG_KNX_EXTNDED_FRAME_FORMAT_SUPPORT 1

#endif /* KNX_KNXSTACK_INCLUDE_KNX_CONFIG_H_ */

