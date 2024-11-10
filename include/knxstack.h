#ifndef KNXSTACK_H_
#define KNXSTACK_H_
/**
 * \brief The main header of the knxstack
 *
 * \filename knxstack.h
 * \author undead
 * \date 10 lis 2024
 *
 * \license This Source Code Form is subject to the terms of the Mozilla Public
 * 			License, v. 2.0. If a copy of the MPL was not distributed with this
 * 			file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 */

#include <knx_platform.h>

typedef enum {
	KNXS_STATE_UNKNOWN = 0,
	KNXS_STATE_READY,
	KNXS_STATE_BUSY,
	KNXS_STATE_IDLE,

} knx_stack_state_t;


typedef struct {
	knx_stack_state_t state;
	knx_device_t device;
} *knx_stack_t;

knx_stack_t knx_init(knx_device_t device);
int knx_loop(knx_stack_t stack);

#endif /* KNXSTACK_H_ */

