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
#include <sys/time.h>
#include <knx_platform.h>
#define KNX_DEBUG_FILE KNX_DEBUG_PLATFORM

uint32_t get_ticks(void)
{
	  struct timeval time_now;
	  gettimeofday(&time_now, NULL);
	  time_t msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
	return msecs_time;
}

static uint8_t rxbuf[] = {0x00, 0x11, 0x02, 0x00,
						  0x01, 0xe9, 0x00, 0x81,
						  0x31, 0xcc, 0xbc, 0x11,
						  0x02, 0x00, 0x01, 0xe1,
						  0x05, 0x80, 0x30};

static size_t rxbuf_pos = 0;

int _knx_read_byte(knx_device_t *device)
{
	int ret = 0;
	ret  = rxbuf[rxbuf_pos];
	rxbuf_pos++;

	if (rxbuf_pos  >= sizeof(rxbuf))
		rxbuf_pos = 0;

	return ret;
}

size_t _knx_wrtie_data(knx_device_t *device, uint8_t *buf, size_t count)
{
	size_t ret = 0;
	int i;
	KNX_LOG_INF("Write Buffer [");
	for (i = 0; i < count; i++)
		printf("%s0x%02x", i > 0 ? ", ": "", buf[i]);
	printf("]\n");
	return ret;
}

int _knx_flush_data(knx_device_t *device)
{
	rxbuf_pos = 0;
	return 0;
}
int main (int argc, char **argv)
{
	printf("Linux_plaftorm PoC debug 0x%x\n",CONFIG_KNX_DEBUG);

	knx_stack_t *stack;
	char str[] = "device";

	knx_device_t *device = str;
	stack = knx_init(device);
	if (! stack )
	{
		printf("Failed: stack isn't initialized\n ");
		return 1;
	}
	int i= 20;
	do {
		usleep(500); //2 ms
		knx_loop(stack);
		i--;
	} while  (i >= 0);
	return 0;
}

