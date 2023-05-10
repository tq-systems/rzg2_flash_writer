/*
 * Copyright (c) 2015-2019, Renesas Electronics Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Renesas nor the names of its contributors may be
 *     used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "common.h"
#include "dgtable.h"
#include "dgmodul1.h"
#include "devdrv.h"
#include "devdrv.h"
#include "bit.h"
#include "cpudrv.h"
#include "scifdrv.h"

#include "reg_rzg2.h"
#include "boot_init_lbsc.h"

#define	WRFLG	(BIT5)

extern const char *const AllHelpMess[ALL_HELP_MESS_LINE];
extern const com_menu MonCom[COMMAND_UNIT];
extern int32_t  gComNo;
extern char gKeyBuf[64];

uint32_t gFLASH_CS1_ID;

/****************************************************************
	MODULE		: dgHelp				*
	FUNCTION	: HELP	MESSAGE				*
	COMMAND		: H					*
	INPUT PARAMETER	: H					*
*****************************************************************/
void	dgHelp(void)
{
	char tmp[64],chPtr,helpNo;

	chPtr = 0;
	if (!GetStrBlk(gKeyBuf,tmp,&chPtr,0))
	{
		PutMess(AllHelpMess);
	}
}



int32_t	GetStr_ByteCount(char *str,uint32_t getByteCount)
{
	uint32_t	byteCount;
	int32_t i;

	for(byteCount = 1; byteCount <= getByteCount;byteCount = byteCount + 1)
	{
		i = 1;
		while(i == 1)
		{
			i = GetChar(str);
		}
		str++;
		i = 1;
		while(i == 1)
		{
			i = GetChar(str);
		}
		str++;
	}
	*str = 0;
}

void	dgScifSpeedUp(void)
{
	dgScifSpeedUp_921600();
}

/****************************************************************
	MODULE		: dgScifSpeedUp				*
	FUNCTION	: Scif speed UP	Change 921.6kbps	*
	COMMAND		: SUP					*
	INPUT PARAMETER	: SUP					*
*****************************************************************/
void	dgScifSpeedUp_921600(void)
{
	uint16_t setData;
#if defined RZG2_HIHOPE || defined TQMARZG2X
	uint32_t product;
#endif /* RZG2_HIHOPE || TQMARZG2X*/
#ifdef RZG2_EK874
	uint32_t sscg;
	uint32_t md;

	md = *((volatile uint32_t*)RST_MODEMR);
	sscg = (md & 0x00001000) >> 12;
#endif /* RZG2_EK874 */

	PutStr("Scif speed UP",1);
	PutStr("Please change to 921.6Kbps baud rate setting of the terminal.",1);
	WaitPutCharSendEnd();

#if defined RZG2_HIHOPE || defined TQMARZG2X
	product = *((volatile uint32_t*)PRR) & (PRR_PRODUCT_MASK | PRR_CUT_MASK);
	setData =0x12;		/* 266.66MHz / (921600*16) = 18.08  @S3D1 */
#endif /* RZG2_HIHOPE || TQMARZG2X*/
#ifdef RZG2_EK874
	if (sscg == 0x0)
	{			/* MD12=0 (SSCG off) �F S3D1C=266.6MHz */
		setData =0x12;	/* 266.66MHz / (921600*16) = 18.08  @S3D1 */
	}
	else
	{			/* MD12=1 (SSCG on)  �F S3D1C=240MHz */
		setData =0x10;	/* 240MHz    / (921600*16) = 16.28  @S3D1C */
	}
#endif /* RZG2_EK874 */
	SetScif2_DL(setData);
}

void	dgReset(void)
{
	volatile uint32_t val;

#ifdef RZG2_EK874
	*((volatile uint32_t*)RST_CA53BAR)  = 0x02;
#else
	*((volatile uint32_t*)RST_CA57BAR)  = 0x02;
#endif
	*((volatile uint32_t*)RST_WDTRSTCR) = 0xA55AC002;

	*((volatile uint32_t*)RWDT_RWTCSRA) = 0xA5A5A500;
	SoftDelay(10);
	*((volatile uint32_t*)RWDT_RWTCNT)  = 0x5A5AFFFF;
	*((volatile uint32_t*)RWDT_RWTCSRA) = 0xA5A5A507;
	*((volatile uint32_t*)RWDT_RWTCSRB) = 0xA5A5A500;
	do
	{
		val = *((volatile uint32_t*)RWDT_RWTCSRA);
	} while(val & WRFLG);
	*((volatile uint32_t*)RWDT_RWTCSRA) = 0xA5A5A587;
//	*((volatile uint32_t*)RWDT_RWTCNT)  = 0x5A5AFFFF;
}
