/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2023 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * Author: Paul Gerber
 *
 */

#include <mmio.h>
#include <stdint-gcc.h>
#include "boot_init_dram_regdef.h"
#include "boot_init_dram_config.h"

uint32_t boardcnf_get_brd_type(void)
{
	uint32_t Prr_Product;
	uint32_t judge = 0;
	uint32_t reg;

	reg = mmio_read_32(PRR);
	Prr_Product = reg & PRR_PRODUCT_MASK;

	switch (Prr_Product) {
		case PRR_PRODUCT_G2N:
			judge = 0;
			break;
		case PRR_PRODUCT_G2M:
			judge = 1;
			break;
		case PRR_PRODUCT_G2H:
			judge = 2;
			break;
	}

	return (judge);
}

const struct _boardcnf boardcnfs[BOARDNUM] = {
/*
 * boardcnf[0] TQMaRZG2N 8Gbit/1Rank/2ch --> 16 Gbit total per Chip --> 16 Gbit total on module = 2 GB
 */
{
	0x01,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3210,
/*dq_swap[]*/		{ 0x17602534, 0x23546701, 0x43257160, 0x10765243 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
	}
},

/*
 * boardcnf[1] TQMaRZG2M-AA 8Gbit/1Rank/2ch --> 16 Gbit total per Chip --> 16 Gbit total on module = 2 GB
 */
{
	0x01,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3210,
/*dq_swap[]*/		{ 0x17602534, 0x23546701, 0x43257160, 0x10765243 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
	}
},

/*
 * boardcnf[2] TQMaRZG2H 8Gbit/1Rank/2ch --> 16 Gbit total per Chip --> 32 Gbit total on module = 4 GB
 */
{
	0x05,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3210,
/*dq_swap[]*/		{ 0x17602534, 0x23541067, 0x43257160, 0x10765243 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3210,
/*dq_swap[]*/		{ 0x17502643, 0x54236170, 0x34521607, 0x07615243 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[2]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3210,
/*dq_swap[]*/		{ 0x17502643, 0x54236170, 0x34521607, 0x07615243 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[3]*/	{ // Dummy
/*ddr_density[]*/	{ 0xff, 0xff },
/*ca_swap*/		0,
/*dqs_swap*/		0,
/*dq_swap[]*/		{ 0, 0, 0, 0 },
/*dm_swap[]*/		{ 0, 0, 0, 0 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}

	}
}
};
