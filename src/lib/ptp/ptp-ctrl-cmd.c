/******************************************************************************
 * Copyright (c) 2017, NXP Semiconductors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
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
 *****************************************************************************/
#include <string.h>
#include <inttypes.h>
/* These are our own include files */
#include <lib/include/ptp.h>
#include <lib/include/gtable.h>
#include <lib/include/status-tables.h>
#include <lib/include/spi.h>
#include <common.h>

static void sja1105_ptp_ctrl_cmd_access(
		void *buf,
		struct sja1105_ptp_ctrl_cmd *ptp_control,
		int write)
{
	int  (*pack_or_unpack)(void*, uint64_t*, int, int, int);
	int    size = 4;

	if (write == 0) {
		pack_or_unpack = gtable_unpack;
		memset(ptp_control, 0, sizeof(*ptp_control));
	} else {
		pack_or_unpack = gtable_pack;
		memset(buf, 0, size);
	}
	pack_or_unpack(buf, &ptp_control->valid,              31, 31, 4);
	pack_or_unpack(buf, &ptp_control->start_schedule,     30, 30, 4);
	pack_or_unpack(buf, &ptp_control->stop_schedule,      29, 29, 4);
	pack_or_unpack(buf, &ptp_control->start_pin_toggle,   28, 28, 4);
	pack_or_unpack(buf, &ptp_control->stop_pin_toggle,    27, 27, 4);
	pack_or_unpack(buf, &ptp_control->reset,               2,  2, 4);
	pack_or_unpack(buf, &ptp_control->ts_based_on_ptpclk,  1,  1, 4);
	pack_or_unpack(buf, &ptp_control->clk_add_mode,        0,  0, 4);
}

void sja1105_ptp_ctrl_cmd_pack(void *buf,
                               struct sja1105_ptp_ctrl_cmd *ptp_control)
{
	sja1105_ptp_ctrl_cmd_access(buf, ptp_control, 1);
}

void sja1105_ptp_ctrl_cmd_unpack(void *buf,
                                 struct sja1105_ptp_ctrl_cmd *ptp_control)
{
	sja1105_ptp_ctrl_cmd_access(buf, ptp_control, 0);
}

void sja1105_ptp_ctrl_cmd_show(struct sja1105_ptp_ctrl_cmd *ptp_control)
{
	printf("VALID      %" PRIX64 "\n", ptp_control->valid);
	printf("PTPSTRTSCH %" PRIX64 "\n", ptp_control->start_schedule);
	printf("PTPSTOPSCH %" PRIX64 "\n", ptp_control->stop_schedule);
	printf("STARTPTPCP %" PRIX64 "\n", ptp_control->start_pin_toggle);
	printf("STOPPTPCP  %" PRIX64 "\n", ptp_control->stop_pin_toggle);
	printf("RESPTP     %" PRIX64 "\n", ptp_control->reset);
	printf("CORRCLK4TS %" PRIX64 "\n", ptp_control->ts_based_on_ptpclk);
	printf("PTPCLKADD  %" PRIX64 "\n", ptp_control->clk_add_mode);
}

/* Wrapper around sja1105_spi_send_packed_buf() */
int sja1105_ptp_ctrl_cmd_send(struct sja1105_spi_setup *spi_setup,
                              struct sja1105_ptp_ctrl_cmd *ptp_control)
{
	const int PTP_CONTROL_ADDR = 0x17;
	const int BUF_LEN = 4;
	uint8_t packed_buf[BUF_LEN];

	sja1105_ptp_ctrl_cmd_pack(packed_buf, ptp_control);
	return sja1105_spi_send_packed_buf(spi_setup,
	                                   SPI_WRITE,
	                                   CORE_ADDR + PTP_CONTROL_ADDR,
	                                   packed_buf,
	                                   BUF_LEN);
};

int sja1105_ptp_start_schedule(struct sja1105_spi_setup *spi_setup)
{
	struct sja1105_ptp_ctrl_cmd ptp_control;

	memset(&ptp_control, 0, sizeof(ptp_control));
	ptp_control.valid = 1;
	ptp_control.start_schedule = 1;
	return sja1105_ptp_ctrl_cmd_send(spi_setup, &ptp_control);
}

int sja1105_ptp_stop_schedule(struct sja1105_spi_setup *spi_setup)
{
	struct sja1105_ptp_ctrl_cmd ptp_control;

	memset(&ptp_control, 0, sizeof(ptp_control));
	ptp_control.valid = 1;
	ptp_control.stop_schedule = 1;
	return sja1105_ptp_ctrl_cmd_send(spi_setup, &ptp_control);
}

int sja1105_ptp_start_pin_toggle(struct sja1105_spi_setup *spi_setup)
{
	struct sja1105_ptp_ctrl_cmd ptp_control;

	memset(&ptp_control, 0, sizeof(ptp_control));
	ptp_control.valid = 1;
	ptp_control.start_pin_toggle = 1;
	return sja1105_ptp_ctrl_cmd_send(spi_setup, &ptp_control);
}

int sja1105_ptp_stop_pin_toggle(struct sja1105_spi_setup *spi_setup)
{
	struct sja1105_ptp_ctrl_cmd ptp_control;

	memset(&ptp_control, 0, sizeof(ptp_control));
	ptp_control.valid = 1;
	ptp_control.stop_pin_toggle = 1;
	return sja1105_ptp_ctrl_cmd_send(spi_setup, &ptp_control);
}

int sja1105_ptp_reset(struct sja1105_spi_setup *spi_setup)
{
	struct sja1105_ptp_ctrl_cmd ptp_control;

	memset(&ptp_control, 0, sizeof(ptp_control));
	ptp_control.valid = 1;
	ptp_control.reset = 1;
	logv("Resetting PTP clock");
	return sja1105_ptp_ctrl_cmd_send(spi_setup, &ptp_control);
}

int sja1105_ptp_set_add_mode(struct sja1105_spi_setup *spi_setup,
                             enum sja1105_ptp_clk_add_mode mode)
{
	struct sja1105_ptp_ctrl_cmd ptp_control;

	memset(&ptp_control, 0, sizeof(ptp_control));
	ptp_control.valid = 1;
	ptp_control.clk_add_mode = mode;
	return sja1105_ptp_ctrl_cmd_send(spi_setup, &ptp_control);
}
