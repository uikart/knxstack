/**
 * \brief |
 *
 * \filename knxstack.c
 * \author undead
 * \date 10 lis 2024
 *
 * \license This Source Code Form is subject to the terms of the Mozilla Public
 * 			License, v. 2.0. If a copy of the MPL was not distributed with this
 * 			file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */


#include <knx_config.h>
#include <knxstack.h>
#include <knx_log.h>

#define KNX_DEBUG_FILE KNX_DEBUG_PLATFORM

knx_stack_t knx_init(knx_device_t device)
{
	knx_stack_t stack = malloc(sizeof(knx_stack_t));
	if (!stack)
		return NULL;

	memset(stack, 0 , sizeof(knx_stack_t));
	stack->device = device;
	stack->state = KNXS_STATE_READY;

	KNX_LOG_ERR("test\n");
	return stack;
}

int knx_loop(knx_stack_t stack)
{
	KNX_LOG_DBG("->> START LOOP: stack %p, stack->device %p\n", stack, stack->device);
	if (!stack || !stack->device) {
		KNX_LOG_ERR("ERROR: device isn't initialized properly\n");
		return -1;
	}
	if (stack->state != KNXS_STATE_READY) {
		KNX_LOG_ERR("ERROR: knx-stack has wrong state\n");
		return -1;
	}

	return 0;
}
