/**
 * \brief |
 *
 * \filename knx_log.h
 * \author undead
 * \date 10 lis 2024
 *
 * \license This Source Code Form is subject to the terms of the Mozilla Public
 * 			License, v. 2.0. If a copy of the MPL was not distributed with this
 * 			file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 */

#ifndef KNX_KNXSTACK_KNX_LOG_H_
#define KNX_KNXSTACK_KNX_LOG_H_
#include "knx_config.h"
#define KNX_DEBUG_L1		(1 << 0)
#define KNX_DEBUG_L2		(1 << 1)
#define KNX_DEBUG_PKG_DUMP	(1 << 2)
#define KNX_DEBUG_PLATFORM	(1 << 3)

#define KNX_CUSTOM_LOG_PREFIX "KNX-S:"
/*
 * The CONFIG_KNX_DEBUG is a global variable that include Enabled Debug Level in the project
 * the KNX_DEBUG_FILE is a define defined in the source_file
 */
#define KNX_DEBUG_IS_ENABLED ((CONFIG_KNX_DEBUG & KNX_DEBUG_FILE) != 0)

#define KNX_LOG_LEVEL_NONE 0U
#define KNX_LOG_LEVEL_ERR  1U
#define KNX_LOG_LEVEL_WRN  2U
#define KNX_LOG_LEVEL_INF  3U
#define KNX_LOG_LEVEL_DBG  4U

# define KNX_LOG(level, ...) ({\
  if ((CONFIG_KNX_DEBUG_LEVEL >= level) && KNX_DEBUG_IS_ENABLED) { printf("%s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__);} \
})


#define KNX_LOG_DBG(...) KNX_LOG(KNX_LOG_LEVEL_DBG, KNX_CUSTOM_LOG_PREFIX __VA_ARGS__)
#define KNX_LOG_INF(...) KNX_LOG(KNX_LOG_LEVEL_INF, KNX_CUSTOM_LOG_PREFIX __VA_ARGS__)
#define KNX_LOG_WRN(...) KNX_LOG(KNX_LOG_LEVEL_WRN, KNX_CUSTOM_LOG_PREFIX __VA_ARGS__)
#define KNX_LOG_ERR(...) KNX_LOG(KNX_LOG_LEVEL_ERR, KNX_CUSTOM_LOG_PREFIX __VA_ARGS__)
#endif /* KNX_KNXSTACK_KNX_LOG_H_ */
