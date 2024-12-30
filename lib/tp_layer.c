/**
 * \brief |
 *
 * \filename TP_layer.c
 * \author undead
 * \date 10 lis 2024
 *
 * \license This Source Code Form is subject to the terms of the Mozilla Public
 * 			License, v. 2.0. If a copy of the MPL was not distributed with this
 * 			file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 */

#include <knxstack.h>
#include <knx_types.h>
#include <knx_data_layer.h>
#include <knx_log.h>

#define KNX_DEBUG_FILE KNX_DEBUG_L1

/* all values in the ms  */
#define L2_EOP_TIMEOUT           2   /* End of Packet timeout */
#define L2_EOPR_TIMEOUT          80   /* End of Packet Remote timeout */
#define L2_CONFIRM_TIMEOUT       500 /* Confirm timeout */
#define L2_RESET_TIMEOUT         100 /* Reset timeout */



/* services Host -> Controller : internal commands, device specific */
#define U_RESET_REQ          0x01
#define U_STATE_REQ          0x02
#define U_SET_BUSY_REQ       0x03
#define U_QUIT_BUSY_REQ      0x04
#define U_BUSMON_REQ         0x05
#define U_SET_ADDRESS_REQ    0xF1
#define U_SET_REPETITION_REQ 0xF2
#define U_L_DATA_OFFSET_REQ  0x08
#define U_SYSTEM_STATE       0x0D
#define U_STOP_MODE_REQ      0x0E
#define U_EXIT_STOP_MODE_REQ 0x0F
#define U_ACK_REQ            0x10
#define U_ACK_REQ_NACK       0x04
#define U_ACK_REQ_BUSY       0x02
#define U_ACK_REQ_ADRESSED   0x01
#define U_CONFIGURE_REQ      0x18
#define U_INT_REG_WR_REQ     0x28
#define U_INT_REG_RD_REQ     0x38
#define U_POLLING_STATE_REQ  0xE0

/* Transmit data */
#define U_L_DATA_START_CONT_REQ 0x80 //-0xBF
#define U_L_DATA_END_REQ        0x40 //-0x7F

/*Services to host controller */

/* DLL services (device is transparent) */
#define L_DATA_STANDARD_IND 0x90
#define L_DATA_EXTENDED_IND 0x10
#define L_DATA_MASK         0xD3
#define L_POLL_DATA_IND     0xF0

/* acknowledge services (device is transparent in bus monitor mode) */
#define L_ACKN_IND          0x00
#define L_ACKN_MASK         0x33
#define L_DATA_CON          0x0B
#define L_DATA_CON_MASK     0x7F
#define SUCCESS             0x80

/* Control services, Device specific */
#define U_RESET_IND           0x03
#define U_STATE_IND           0x07
#define SLAVE_COLLISION       0x80
#define RECEIVE_ERROR         0x40
#define TRANSMIT_ERROR        0x20
#define PROTOCOL_ERROR        0x10
#define TEMPERATURE_WARNING   0x08
#define U_FRAME_STATE_IND     0x13
#define U_FRAME_STATE_MASK    0x17
#define PARITY_BIT_ERROR      0x80
#define CHECKSUM_LENGTH_ERROR 0x40
#define TIMING_ERROR          0x20
#define U_CONFIGURE_IND       0x01
#define U_CONFIGURE_MASK      0x83
#define AUTO_ACKNOWLEDGE      0x20
#define AUTO_POLLING          0x10
#define CRC_CCITT             0x80
#define FRAME_END_WITH_MARKER 0x40
#define U_FRAME_END_IND       0xCB
#define U_STOP_MODE_IND       0x2B
#define U_SYSTEM_STAT_IND     0x4B

/* ptr - pointer to buffer with minimum two bytes ptr and ptr+1 */

uint16_t _ntohs(uint16_t value)
{
	return (((value & 0xff) << 8) | ((value & 0xff00) >> 8));
}

/* The rule: always t1 > t2 */
uint32_t diff_time(uint32_t t1, uint32_t t2) {
//	KNX_LOG_DBG("t1 = %u t2 = %u\n", t1, t2);
	return (t1 >= t2 ? t1 - t2: (uint32_t)((uint64_t)((uint32_t)(-1)+ t2) - t1));
}


int tp_layer_reader(knx_data_layer_t *dl)
{
//	KNX_LOG_DBG("TP: state RX:%x(time %d): knx_phy_state_run\n", dl->rx_state,get_ticks() );
	uint8_t byte, conn_msg_byte;
	uint32_t time2;
	switch (dl->rx_state) {
	case knx_phy_rx_wait_start:
		byte = (uint8_t)_knx_read_byte(dl->device);
		dl->last_rx_byte_time = get_ticks();
		dl->rx_count = 0;
		dl->rx_xor_sum = 0;
		time2 = get_ticks();
		//KNX_LOG_DBG("(time %d last_rx_byte_time %d)\n", time2 ,dl->last_rx_byte_time  );

		if ((byte & L_DATA_MASK) == L_DATA_STANDARD_IND) {
//			KNX_LOG_DBG("Telegram SD\n");
			dl->rx_buf[dl->rx_count++] = byte;
			dl->rx_xor_sum ^= byte;
			dl->rx_state  = knx_phy_rx_l_addr;
			break;
		} else if ((byte & L_DATA_MASK) == L_DATA_EXTENDED_IND) {
			KNX_LOG_DBG("Telegram EX\n");
			dl->rx_buf[dl->rx_count++] = byte;
			dl->rx_xor_sum ^= byte;
			dl->rx_state  = knx_phy_rx_ctlre;
			break;
		} else if ((byte & L_DATA_CON_MASK) == L_DATA_CON) {
			conn_msg_byte = byte;
		} else if (byte == L_POLL_DATA_IND) {
			/*TODO undead: to fill */
			KNX_LOG_DBG("L_POLL_DATA_IND\n");
		} else if ((byte & L_ACKN_MASK) == L_ACKN_IND) {
			/*TODO undead: to fill */
			KNX_LOG_DBG("L_ACKN_IND\n");
		} else if (byte == U_RESET_IND) {
			/*TODO undead: to fill */
			KNX_LOG_DBG("U_RESET_IND\n");
		} else if ((byte & U_STATE_IND) == U_STATE_IND) {
			/*TODO undead: to fill */
			KNX_LOG_DBG("U_STATE_IND:%s%s%s%s%s\n", byte & 0x80 ? "SC":"..",byte & 0x40 ? "RE":"..",
						byte & 0x20 ? "TE":"..", byte & 0x10 ? "PE":"..", byte & 0x08 ? "TW":"..");
		} else if ((byte & U_FRAME_STATE_MASK) == U_FRAME_STATE_IND) {
			/*TODO undead: to fill */
			KNX_LOG_DBG("U_FRAME_STATE_IND: 0x%02x\n",byte);
		} else if ((byte & U_CONFIGURE_MASK) == U_CONFIGURE_IND) {
			/*TODO undead: to fill */
			KNX_LOG_DBG("U_CONFIGURE_IND: 0x%02x\n", byte);
		} else if (byte == U_FRAME_END_IND) {
			/*TODO undead: to fill */
			KNX_LOG_DBG("U_FRAME_END_IND\n");
		} else if (byte == U_STOP_MODE_IND) {
			/*TODO undead: to fill */
			KNX_LOG_DBG("U_STOP_MODE_IND\n");
		} else if (byte == U_SYSTEM_STAT_IND) {
			/*TODO undead: to fill */
			KNX_LOG_DBG("U_SYSTEM_STAT_IND\n");
		} else {
			KNX_LOG_DBG("got UNEXPECTED: 0x%02x\n", byte);
		}
		break;


#if 0
	case RX_L_ADDR:
        if (millis() - _lastByteRxTime > EOPR_TIMEOUT)
        {
            _rxState = RX_WAIT_START;
            println("EOPR @ RX_L_ADDR");
            break;
        }
        if (!_platform.uartAvailable())
            break;
        _lastByteRxTime = millis();
        rxByte = _platform.readUart();
#ifdef DBG_TRACE
        print(rxByte, HEX);
#endif
        buffer[_RxByteCnt++] = rxByte;
        _xorSum ^= rxByte;

        if (_RxByteCnt == 7)
        {
            //Destination Address + payload available
            //check if echo; ignore repeat bit of control byte
            _isEcho = (_sendBuffer != nullptr && (!((buffer[0] ^ _sendBuffer[0]) & ~0x20) && !memcmp(buffer + _convert + 1, _sendBuffer + 1, 5)));

            //convert into Extended.ind
            if (_convert)
            {
                buffer[1] = buffer[6] & 0xF0;
                buffer[6] &= 0x0F;
            }

            if (!_isEcho)
            {
                uint8_t c = U_ACK_REQ;

                // The bau knows everything and could either check the address table object (normal device)
                // or any filter tables (coupler) to see if we are addressed.

                //check if individual or group address
                bool isGroupAddress = (buffer[1] & 0x80) != 0;
                uint16_t addr = getWord(buffer + 4);

                if (_cb.isAckRequired(addr, isGroupAddress))
                {
                    c |= U_ACK_REQ_ADRESSED;
                }

                // Hint: We can send directly here, this doesn't disturb other transmissions
                // We don't have to update _lastByteTxTime because after U_ACK_REQ the timing is not so tight
                _platform.writeUart(c);
            }
            _rxState = RX_L_DATA;
        }
        break;
 #endif
	case knx_phy_rx_ctlre:
		if (diff_time(get_ticks(), dl->last_rx_byte_time) > L2_EOPR_TIMEOUT) {
			KNX_LOG_WRN("L2_EOPR_TIMEOUT\n");
			dl->rx_state = knx_phy_rx_wait_start;
			break;
		}

		byte = _knx_read_byte(dl->device);
		dl->last_rx_byte_time = get_ticks();

		dl->rx_buf[dl->rx_count++] = byte;
		dl->rx_xor_sum ^= byte;
		dl->rx_state  = knx_phy_rx_l_addr;
		break;
	case knx_phy_rx_l_addr:
//		KNX_LOG_DBG("(time %d)\n", get_ticks() );
//		KNX_LOG_DBG("TP: state RX %x: knx_phy_rx_l_addr\n", dl->rx_state);
		if (diff_time(get_ticks(), dl->last_rx_byte_time) > L2_EOPR_TIMEOUT) {
			KNX_LOG_WRN("L2_EOPR_TIMEOUT\n");
			dl->rx_state = knx_phy_rx_wait_start;
			break;
		}

		byte = _knx_read_byte(dl->device);
		dl->last_rx_byte_time = get_ticks();

		dl->rx_buf[dl->rx_count++] = byte;
		dl->rx_xor_sum ^= byte;

		/* The basic frame is (control byte src addr, dst addr, flags 7 bytes*/
		if (dl->rx_count == 7) {

			uint8_t c = U_ACK_REQ;
			L_Data_Standard_Frame_t *std_frame = (L_Data_Standard_Frame_t*)dl->rx_buf;
			std_frame->src_address.raw_addr = _ntohs(std_frame->src_address.raw_addr);
			std_frame->dst_address.raw_addr = _ntohs(std_frame->dst_address.raw_addr);
/*
			int is_group_address = std_frame->atlg.fields.AT;
			knx_address_t src = {raw_addr: std_frame->src_address.raw_addr};
			knx_address_t dst = {raw_addr: std_frame->dst_address.raw_addr};
*/
			/*
			if (is_group_address)
				KNX_LOG_DBG("group dst address %d.%d\n", dst.group.main, dst.group.sub);
			else
				KNX_LOG_DBG("dst address %d.%d.%d\n", dst.physical.area, dst.physical.line, dst.physical.device);
			*/
/*
				if (_cb.isAckRequired(addr, isGroupAddress)) {
					c |= U_ACK_REQ_ADRESSED;
				}

				// Hint: We can send directly here, this doesn't disturb other transmissions
				// We don't have to update _lastByteTxTime because after U_ACK_REQ the timing is not so tight
				_platform.writeUart(c);
			}
			*/
			dl->rx_state = knx_phy_rx_l_data;
		}
		break;
	case knx_phy_rx_l_data:
//KNX_LOG_DBG("(time %d)\n", get_ticks() );
		//KNX_LOG_DBG("TP: state RX %x: knx_phy_rx_l_data\n", dl->rx_state);
//dsk-p----------------------------------

		byte = _knx_read_byte(dl->device);
		dl->last_rx_byte_time = get_ticks();


		if (dl->rx_count == KNX_TELEGRAM_MAX_SIZE - 2)
		{
			KNX_LOG_WRN("invalid telegram size");
			dl->rx_state = knx_phy_rx_wait_eop;
			break;
		} else {
			dl->rx_buf[dl->rx_count++] = byte;
		}

		if (dl->rx_count == dl->rx_buf[6] + 7 + 2) /* magic values to change to Defines */
		{
			if (byte == (uint8_t)(~dl->rx_xor_sum))
			{
				/* TODO: check if echo present if not then run callback */
				if (!dl->rx_echo)
				{
					/* call callback from upper layer */
					dl->frame_received_cb(dl, dl->frame_received_cb_userdata, dl->rx_buf, dl->rx_count);
				}
				dl->rx_state = knx_phy_rx_wait_start;
			} else 	{
				KNX_LOG_WRN("Invalid Check sum. Ignoring frame...\n");
				dl->rx_state = knx_phy_rx_wait_eop;
				break;
			}
		} else {
			dl->rx_xor_sum ^= byte;
		}
		break;
	case knx_phy_rx_wait_eop:
//		KNX_LOG_DBG("(time %d)\n", get_ticks() );
//		KNX_LOG_DBG("TP: state RX %x: knx_phy_rx_wait_eop\n", dl->rx_state);
		_knx_flush_data(dl->device);
		dl->rx_state = knx_phy_rx_wait_start;
		dl->last_rx_byte_time = get_ticks();
		break;
	default:
		/* By default enter to wait to the end of the packet */
		dl->rx_state = knx_phy_rx_wait_eop;
		return -1;
	}
	return 0;
}

int tp_layer_writer(knx_data_layer_t *dl)
{
//	KNX_LOG_DBG("TP: state TX %x: knx_phy_state_run\n", dl->tx_state);
	switch (dl->tx_state) {
	case knx_phy_tx_idle:
		break;
	case knx_phy_tx_frame:
		break;
	case knx_phy_tx_wait_conn:
		break;
	default:
		return -1;
	}
	return 0;
}

int tp_layer_loop(void *data_layer)
{
	knx_data_layer_t *dl = (knx_data_layer_t *)data_layer;
	switch (dl->state) {
	case knx_phy_state_run:
//		KNX_LOG_DBG("TP: state %x: knx_phy_state_run\n", dl->state);
		do {
			tp_layer_reader(dl);
			/* TOOD undead add the function witch will check if is any data in the buffer */
		} while (dl->rx_state == knx_phy_rx_l_addr  /* and until we have data in the buffers */);
		tp_layer_writer(dl);
		break;
	default:
//		KNX_LOG_DBG("TP: state: %u\n", dl->state);
	}
	return 0;
}


knx_data_layer_t *knx_data_layer_init(void *stack, frame_received_callback_t frame_received_callback, void *frcb_userdata)
{
	knx_stack_t *knx_stack = (knx_stack_t*)stack;
	knx_data_layer_t *data_layer = malloc(sizeof(knx_data_layer_t));
	if (!data_layer)
		return NULL;

	memset(data_layer, 0, sizeof(knx_data_layer_t));

	data_layer->stack = knx_stack;
	data_layer->device = knx_stack->device;

	data_layer->loop = &tp_layer_loop;

	data_layer->frame_received_cb = frame_received_callback;
	data_layer->frame_received_cb_userdata = frcb_userdata;

	data_layer->state = knx_phy_state_run;

	return data_layer;
}

