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
#include "internal.h"

int
ptp_config_write(xmlTextWriterPtr writer,
                 struct sja1105_ptp_config *config)
{
	int rc = 0;

	logv("writing PTP configuration");
	rc |= xmlTextWriterStartElement(writer, BAD_CAST "ptp");
	rc |= xml_write_field(writer, "pin_duration", config->pin_duration);
	rc |= xml_write_field(writer, "pin_start", config->pin_start);
	rc |= xml_write_field(writer, "schedule_time", config->schedule_time);
	rc |= xml_write_field(writer, "schedule_correction_period",
	                      config->schedule_correction_period);
	rc |= xml_write_field(writer, "ts_based_on_ptpclk",
	                      config->ts_based_on_ptpclk);
	rc |= xml_write_field(writer, "schedule_autostart",
	                      config->schedule_autostart);
	rc |= xml_write_field(writer, "pin_toggle_autostart",
	                      config->pin_toggle_autostart);
	rc |= xmlTextWriterEndElement(writer);
	return rc;
}

