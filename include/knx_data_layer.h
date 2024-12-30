/**
 * \brief |
 *
 * \filename knx_data_layer.h
 * \author undead
 * \date 10 lis 2024
 *
 * \license This Source Code Form is subject to the terms of the Mozilla Public
 * 			License, v. 2.0. If a copy of the MPL was not distributed with this
 * 			file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 */

#ifndef KNX_KNXSTACK_INCLUDE_KNX_DATA_LAYER_H_
#define KNX_KNXSTACK_INCLUDE_KNX_DATA_LAYER_H_

#include <stdint.h>
#include "knx_config.h"
#include <knxstack.h>

#if !defined(CONFIG_KNX_EXTNDED_FRAME_FORMAT_SUPPORT)
#    define KNX_TELEGRAM_MAX_SIZE 22
#else
#    define KNX_TELEGRAM_MAX_SIZE 269
#endif

typedef enum {
	knx_phy_state_reset = 0,
	knx_phy_state_power_up,
	knx_phy_state_stop,
	knx_phy_state_sync,
	knx_phy_state_idle,
	knx_phy_state_run,

} knx_phy_state_t;

typedef enum {
	knx_phy_rx_wait_start = 0,
	knx_phy_rx_ctlre,
	knx_phy_rx_l_addr,
	knx_phy_rx_l_data,
	knx_phy_rx_wait_eop,
} knx_phy_rx_state_t;

typedef enum {
	knx_phy_tx_idle = 0,
	knx_phy_tx_frame,
	knx_phy_tx_wait_conn
} knx_phy_tx_state_t;

typedef int (*loop_func_t) (void *data_layer);
typedef size_t (*frame_received_callback_t) (void *data_layer, void *userdata, char *buf, size_t bufsize);

typedef struct {
	uint8_t rx_buf[KNX_TELEGRAM_MAX_SIZE];
	bool rx_echo;
	uint16_t rx_count;
	uint8_t rx_xor_sum;
	uint8_t tx_buf[KNX_TELEGRAM_MAX_SIZE];
	uint16_t tx_count;

	knx_phy_state_t state;
	knx_phy_rx_state_t rx_state;
	knx_phy_tx_state_t tx_state;
	uint32_t last_rx_byte_time;

	void *stack;
	const knx_device_t *device;
	void *frame_received_cb_userdata;

	loop_func_t loop;
	frame_received_callback_t frame_received_cb;

} knx_data_layer_t;


knx_data_layer_t *knx_data_layer_init(void *stack, frame_received_callback_t frame_received_callback, void *frcb_userdata);


#endif /* KNX_KNXSTACK_INCLUDE_KNX_DATA_LAYER_H_ */
