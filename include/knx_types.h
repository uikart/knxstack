/**
 * \brief |
 *
 * \filename knx_types.h
 * \author undead
 * \date 10 lis 2024
 *
 * \license This Source Code Form is subject to the terms of the Mozilla Public
 * 			License, v. 2.0. If a copy of the MPL was not distributed with this
 * 			file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 */
#ifndef KNX_KNXSTACK_INCLUDE_KNX_TYPES_H_
#define KNX_KNXSTACK_INCLUDE_KNX_TYPES_H_

#include <stdint.h>

typedef enum
{
	L_Data_Extended = 0x0,
	L_Data_Standard = 0x1
} knx_frame_format_t;

typedef enum
{
	f_low_priority = 0x3, /* Normal priority of group communication, mandatory for long Frames, burst traffic */
	f_normal_priority = 0x1, /* More important telegrams like central functions */
	f_urgent_priority = 0x2, /* Used for alarms, Reserved for urgent frames */
	f_system_priority = 0x0, /* Mainly used by ETS for device programming, or System frames */
#if 0
	f_invalid = 0x4, /* Used for transmission */
	f_repeted = 0x5 /* Used for transmission */
#endif
} knx_frame_priority_t;

typedef enum
{
	at_individual = 0x0,
	at_group = 0x1
} knx_address_type_t;

typedef union {
	struct {
		uint16_t device:8;
		uint16_t line:4;
		uint16_t area:4;
	}physical;
	struct {
		uint16_t sub:11;
		uint16_t main:4;
		uint16_t reserved:1;
	}group;
	uint16_t raw_addr;
} knx_address_t;

typedef union {
	struct {
		uint8_t reserved3:2;
		knx_frame_priority_t priority:2;
		uint8_t reserved2:1;
		bool reapeted:1;
		uint8_t reserved1:1;
		knx_frame_format_t frame_type:1;
	}fields;
	uint8_t raw;
} knx_control_field_t;

typedef union {
	struct {
		uint8_t lsdu_length:4;
		uint8_t hop_count:3;
		knx_address_type_t AT:1;
	}fields;
	uint8_t raw;
} knx_atlg_ctrl_t;

typedef union {
	struct {
		uint8_t eff:4;
		uint8_t hop_count:3;
		knx_address_type_t AT:1;
	}fields;
	uint8_t raw;
} knx_extended_control_field_t;

typedef enum
{
	n_ok = 0x0,
	n_not_ok = 0x1
} N_Status_t;

enum HopCountType
{
	NetworkLayerParameter, /* use N_hop_count */
	UnlimitedRouting = 7 /* hopCount is set to 7. This means that the frame never expires. This
							could be a problem if your bus contains a circle. */
};

typedef enum
{
	t_ok = 0x0,
	t_not_ok = 0x1
} T_Status;


typedef enum
{
	a_ok = 0x0,
	a_not_ok = 0x1
} A_Status;

typedef struct {
	knx_control_field_t ctrl;
	knx_address_t src_address;
	knx_address_t dst_address;
	knx_atlg_ctrl_t	atlg;
}__attribute__((packed)) L_Data_Standard_Frame_t;

typedef struct {
	knx_control_field_t ctrl;
	knx_extended_control_field_t ctrle;
	knx_address_t src_address;
	knx_address_t dst_address;
	uint8_t	length;
}__attribute__((packed)) L_Data_Extended_Frame_t;

typedef struct {
	uint8_t ctrl;
	knx_address_t src_address;
	knx_address_t dst_address;
	uint8_t	expected_data;
	uint8_t checksum;
}__attribute__((packed)) L_Data_Poll_Frame_t;


#endif /* KNX_KNXSTACK_INCLUDE_KNX_TYPES_H_ */
