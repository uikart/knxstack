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
#include <knx_types.h>

#define KNX_DEBUG_FILE KNX_DEBUG_PLATFORM

int dump_L_Data_standrad_frame(L_Data_Standard_Frame_t *frame, size_t size)
{
	KNX_LOG_DBG("RCV_StandardFrame:\n");
	KNX_LOG_DBG("	priority      :%u\n", frame->ctrl.fields.priority);
	KNX_LOG_DBG("	frame_type    :%u\n", frame->ctrl.fields.frame_type);
	KNX_LOG_DBG("	frame repeated:%u\n", frame->ctrl.fields.reapeted);
	KNX_LOG_DBG("	Source Address:%u.%u.%u\n", frame->src_address.physical.area,
			frame->src_address.physical.line, frame->src_address.physical.device);
	KNX_LOG_DBG("	DST Adr_type  :%u\n", frame->atlg.fields.AT);
	if ( frame->atlg.fields.AT == at_individual ) {
		KNX_LOG_DBG("	DST AddressIND:%u.%u.%u\n", frame->dst_address.physical.area,
				frame->dst_address.physical.line, frame->dst_address.physical.device);
	} else {
		KNX_LOG_DBG("	DST AddressGRP:%u.%u\n", frame->dst_address.group.main, frame->dst_address.group.sub);
	}
	KNX_LOG_DBG("	Hop count  :%u\n", frame->atlg.fields.hop_count);
	KNX_LOG_DBG("	LDSU len   :%u\n", frame->atlg.fields.lsdu_length);
	KNX_LOG_DBG("   LDSU[");
	char *buf = (char*)(frame+1);
	for (int i = 0; i < frame->atlg.fields.lsdu_length; i++)
		printf("%s0x%02x",i == 0 ? "": ",", buf[i+1]);
	printf("]\n");
	return 0;
}

int process_L_Data_standrad_frame(void *userdata, L_Data_Standard_Frame_t *frame, size_t size)
{
	/* fix the pointers */
	//frame->tpdu = (char *)(frame + offsetof(L_Data_Standard_Frame_t, tpdu));
//	dump_L_Data_standrad_frame(frame, size);
	return 0;
}

size_t frame_received_callback(void *data_layer, void *userdata, char *buf, size_t bufsize)
{

	printf("DUMP telegram\n");
	int i ;
	for ( i = 0; i < bufsize; i++ ) {
		printf("[%d]=0x%02x\n",i, buf[i]);
	}

	knx_control_field_t ctrl;
	ctrl.raw = buf[0];

	if (ctrl.fields.frame_type == L_Data_Standard) {
		process_L_Data_standrad_frame(userdata, (L_Data_Standard_Frame_t *)buf, bufsize);
	} else if (ctrl.fields.frame_type == L_Data_Extended) {
		printf("not supported yet");
	} else if (ctrl.raw == 0xf0) { /* POLL_DATA_FRAME */
		printf("not supported yet");
	}

	return bufsize;
}


knx_stack_t *knx_init(const knx_device_t *device)
{
	knx_stack_t *stack = malloc(sizeof(knx_stack_t));
	if (!stack)
		return NULL;

	memset(stack, 0 , sizeof(knx_stack_t));
	stack->device = device;

	if (!(stack->data_layer = knx_data_layer_init(stack ,&frame_received_callback, NULL))) {
		free(stack);
		stack = NULL;
	}


	stack->state = KNXS_STATE_READY;
	return stack;
}

int knx_loop(knx_stack_t *stack)
{
	//KNX_LOG_DBG("->> START LOOP: stack %p, stack->device %p\n", stack, stack->device);
	if (!stack || !stack->device) {
		KNX_LOG_ERR("ERROR: device isn't initialized properly\n");
		return -1;
	}
	if (stack->state != KNXS_STATE_READY) {
		KNX_LOG_ERR("ERROR: knx-stack has wrong state\n");
		return -1;
	}

	return stack->data_layer->loop(stack->data_layer);
}
