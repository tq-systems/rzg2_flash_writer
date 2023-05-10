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
#include "scifdrv.h"
#include "bit.h"
#include "reg_rzg2.h"
#include "devdrv.h"

/************************************************************************/
/*									*/
/*	Debug Seirial(SCIF2)						*/
/*									*/
/************************************************************************/
int32_t PutCharSCIF2(char outChar)
{
	while(!(0x60 & *((volatile uint16_t*)SCIF2_SCFSR)));
	*((volatile unsigned char*)SCIF2_SCFTDR) = outChar;
	*((volatile uint16_t*)SCIF2_SCFSR) &= ~0x60;	/* TEND,TDFE clear */
	return(0);
}

int32_t GetCharSCIF2(char *inChar)
{
	do
	{
		if (0x91 & *((volatile uint16_t *)SCIF2_SCFSR))
		{
			*((volatile uint16_t *)SCIF2_SCFSR) &= ~0x91;
		}
		if (0x01 & *((volatile uint16_t *)SCIF2_SCLSR))
		{
			PutStr("ORER",1);
			*((volatile uint16_t *)SCIF2_SCLSR) &= ~0x01;
		}
	} while(!(0x02 & *((volatile uint16_t *)SCIF2_SCFSR)));

	*inChar = *((volatile unsigned char*)SCIF2_SCFRDR);
	*((volatile uint16_t*)SCIF2_SCFSR) &= ~0x02;
	return(0);
}

void PowerOnScif2(void)
{
	uint32_t dataL;

	dataL = *((volatile uint32_t*)CPG_MSTPSR3);
	if (dataL & BIT10)
	{
		/* case SCIF2(IrDA) Standby */
		dataL &= ~BIT10;
		*((volatile uint32_t*)CPG_CPGWPR)    = ~dataL;
		*((volatile uint32_t*)CPG_SMSTPCR3)  =  dataL;
		while(BIT10 & *((volatile uint32_t*)CPG_MSTPSR3));  /* wait bit=0 */
	}
}

void WaitPutScif2SendEnd(void)
{
	uint16_t dataW;
	uint32_t loop;

	loop=1;
	while(loop)
	{
		dataW = *((volatile uint16_t*)SCIF2_SCFSR);
		if (dataW & BIT6)
		{
			loop = 0;
		}
	}
}

#if defined RZG2_HIHOPE || defined TQMARZG2X
void InitScif2_SCIFCLK(void)
{
	volatile uint16_t dataW;
	uint32_t prr;

	PowerOnScif2();

	dataW = *((volatile uint16_t*)SCIF2_SCLSR);	/* dummy read     		*/
	*((volatile uint16_t*)SCIF2_SCLSR) = 0x0000;	/* clear ORER bit 		*/
	*((volatile uint16_t*)SCIF2_SCFSR) = 0x0000;	/* clear all error bit	*/

	*((volatile uint16_t*)SCIF2_SCSCR) = 0x0000;	/* clear SCR.TE & SCR.RE*/
	*((volatile uint16_t*)SCIF2_SCFCR) = 0x0006;	/* reset tx-fifo, reset rx-fifo. */
	*((volatile uint16_t*)SCIF2_SCFSR) = 0x0000;	/* clear ER, TEND, TDFE, BRK, RDF, DR */

	*((volatile uint16_t*)SCIF2_SCSCR) = 0x0002;	/* external clock, SC_CLK pin used for input pin */
	*((volatile uint16_t*)SCIF2_SCSMR) = 0x0000;	/* 8bit data, no-parity, 1 stop, Po/1 */
	SoftDelay(100);

	*((volatile uint16_t*)SCIF2_DL)    = 0x0091;	/* 266.66MHz / (115200*16) = 144.67 */
	*((volatile uint16_t*)SCIF2_CKS)   = 0x4000;	/* select S3D1-Clock */

	SoftDelay(100);
	*((volatile uint16_t*)SCIF2_SCFCR) = 0x0000;	/* reset-off tx-fifo, rx-fifo. */
	*((volatile uint16_t*)SCIF2_SCSCR) = 0x0032;	/* enable TE, RE; SC_CLK=input */

	SoftDelay(100);
}
#endif /* RZG2_HIHOPE || TQMARZG2X*/

#ifdef RZG2_EK874
void InitScif2_SCIFCLK_G2E(void)
{
	volatile uint16_t dataW;
	uint32_t md;
	uint32_t sscg;

	PowerOnScif2();

	md = *((volatile uint32_t*)RST_MODEMR);
	sscg = (md & 0x00001000) >> 12;

	dataW = *((volatile uint16_t*)SCIF2_SCLSR);	/* dummy read     		*/
	*((volatile uint16_t*)SCIF2_SCLSR) = 0x0000;	/* clear ORER bit 		*/
	*((volatile uint16_t*)SCIF2_SCFSR) = 0x0000;	/* clear all error bit	*/

	*((volatile uint16_t*)SCIF2_SCSCR) = 0x0000;	/* clear SCR.TE & SCR.RE*/
	*((volatile uint16_t*)SCIF2_SCFCR) = 0x0006;	/* reset tx-fifo, reset rx-fifo. */
	*((volatile uint16_t*)SCIF2_SCFSR) = 0x0000;	/* clear ER, TEND, TDFE, BRK, RDF, DR */

	*((volatile uint16_t*)SCIF2_SCSCR) = 0x0002;	/* external clock, SC_CLK pin used for input pin */
	*((volatile uint16_t*)SCIF2_SCSMR) = 0x0000;	/* 8bit data, no-parity, 1 stop, Po/1 */
	SoftDelay(100);

	if (sscg == 0x0)
	{
		/* MD12=0 (SSCG off) �F S3D1C=266.6MHz */
		*((volatile uint16_t*)SCIF2_DL) = 0x0091;	/* 266.66MHz / (115200*16) = 144.67 */
	}
	else
	{	/* MD12=1 (SSCG on)  �F S3D1C=250MHz */
		*((volatile uint16_t*)SCIF2_DL) = 0x0082;	/* 240.00MHz / (115200*16) = 130.21 */
	}
	*((volatile uint16_t*)SCIF2_CKS)   = 0x4000;	/* select S3D1-Clock */
	SoftDelay(100);
	*((volatile uint16_t*)SCIF2_SCFCR) = 0x0000;	/* reset-off tx-fifo, rx-fifo. */
	*((volatile uint16_t*)SCIF2_SCSCR) = 0x0032;	/* enable TE, RE; SC_CLK=input */

	SoftDelay(100);
}
#endif /* RZG2_EK874 */

void SetScif2_DL(uint16_t setData)
{
	*((volatile uint16_t*)SCIF2_DL)    = setData;
}

void SetScif2_BRR(uint8_t setData)
{
	*((volatile uint8_t*)SCIF2_SCBRR)  = setData;
}


uint32_t SCIF_TerminalInputCheck(char* str)
{
	char result = 0;
	
	if (0x91 & *((volatile uint16_t *)SCIF2_SCFSR))
	{
		*((volatile uint16_t *)SCIF2_SCFSR) &= ~0x91;
	}
	if (0x01 & *((volatile uint16_t *)SCIF2_SCLSR))
	{
		PutStr("ORER",1);
		*((volatile uint16_t *)SCIF2_SCLSR) &= ~0x01;
	}
	if (0x02 & *((volatile uint16_t *)SCIF2_SCFSR))
	{
		*str = *((volatile unsigned char*)SCIF2_SCFRDR);
		*((volatile uint16_t*)SCIF2_SCFSR) &= ~0x02;
		result = 1;
	}
	return result;
}
