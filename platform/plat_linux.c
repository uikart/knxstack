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
#include <unistd.h>
#include <knx_config.h>
#include <knx_log.h>
#include <knxstack.h>

#define KNX_DEBUG_FILE KNX_DEBUG_PLATFORM
int main (int argc, char **argv)
{
	printf("Linux_plaftorm PoC debug 0x%x is debug %d\n",CONFIG_KNX_DEBUG, KNX_DEBUG_IS_ENABLED);

	knx_stack_t stack;
	char str[] = "device";

	knx_device_t device = str;
	KNX_LOG_ERR("err\n");
	stack = knx_init(device);
	if (! stack )
	{
		printf("Failed: stack isn't initialized\n ");
		return 1;
	}
	do {
		usleep(2000); //2 ms
	} while (knx_loop(stack) >= 0);
	return 0;
}

