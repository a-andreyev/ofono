/*
 *  oFono - Open Source Telephony - RIL-based devices
 *
 *  Copyright (C) 2015 Jolla Ltd.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include "ril_util.h"
#include "ril_log.h"
#include "ril_constants.h"

#include <grilio_channel.h>
#include <grilio_parser.h>

#include <sys/socket.h>

#include "common.h"
#include "util.h"

const char *ril_error_to_string(int error)
{
#define RIL_E_(name) case RIL_E_##name: return #name
#define GRILIO_E_(name) case GRILIO_STATUS_##name: return "GRILIO_" #name
	static char unknown[12];
	switch (error) {
	case RIL_E_SUCCESS: return "OK";
	GRILIO_E_(TIMEOUT);
	GRILIO_E_(CANCELLED);
	RIL_E_(RADIO_NOT_AVAILABLE);
	RIL_E_(GENERIC_FAILURE);
	RIL_E_(PASSWORD_INCORRECT);
	RIL_E_(SIM_PIN2);
	RIL_E_(SIM_PUK2);
	RIL_E_(REQUEST_NOT_SUPPORTED);
	RIL_E_(CANCELLED);
	RIL_E_(OP_NOT_ALLOWED_DURING_VOICE_CALL);
	RIL_E_(OP_NOT_ALLOWED_BEFORE_REG_TO_NW);
	RIL_E_(SMS_SEND_FAIL_RETRY);
	RIL_E_(SIM_ABSENT);
	RIL_E_(SUBSCRIPTION_NOT_AVAILABLE);
	RIL_E_(MODE_NOT_SUPPORTED);
	RIL_E_(FDN_CHECK_FAILURE);
	RIL_E_(ILLEGAL_SIM_OR_ME);
	RIL_E_(UNUSED);
	RIL_E_(DIAL_MODIFIED_TO_USSD);
	RIL_E_(DIAL_MODIFIED_TO_SS);
	RIL_E_(DIAL_MODIFIED_TO_DIAL);
	RIL_E_(USSD_MODIFIED_TO_DIAL);
	RIL_E_(USSD_MODIFIED_TO_SS);
	RIL_E_(USSD_MODIFIED_TO_USSD);
	RIL_E_(SS_MODIFIED_TO_DIAL);
	RIL_E_(SS_MODIFIED_TO_USSD);
	RIL_E_(SS_MODIFIED_TO_SS);
	RIL_E_(SUBSCRIPTION_NOT_SUPPORTED);
	RIL_E_(MISSING_RESOURCE);
	RIL_E_(NO_SUCH_ELEMENT);
	RIL_E_(INVALID_PARAMETER);
	default:
		snprintf(unknown, sizeof(unknown), "%d", error);
		return unknown;
	}
}

const char *ril_request_to_string(guint request)
{
#define RIL_REQUEST_(name) case RIL_REQUEST_##name: return #name
	static char unknown[24];
	switch (request) {
	RIL_REQUEST_(GET_SIM_STATUS);
	RIL_REQUEST_(ENTER_SIM_PIN);
	RIL_REQUEST_(ENTER_SIM_PUK);
	RIL_REQUEST_(ENTER_SIM_PIN2);
	RIL_REQUEST_(ENTER_SIM_PUK2);
	RIL_REQUEST_(CHANGE_SIM_PIN);
	RIL_REQUEST_(CHANGE_SIM_PIN2);
	RIL_REQUEST_(ENTER_NETWORK_DEPERSONALIZATION);
	RIL_REQUEST_(GET_CURRENT_CALLS);
	RIL_REQUEST_(DIAL);
	RIL_REQUEST_(GET_IMSI);
	RIL_REQUEST_(HANGUP);
	RIL_REQUEST_(HANGUP_WAITING_OR_BACKGROUND);
	RIL_REQUEST_(HANGUP_FOREGROUND_RESUME_BACKGROUND);
	RIL_REQUEST_(SWITCH_HOLDING_AND_ACTIVE);
	RIL_REQUEST_(CONFERENCE);
	RIL_REQUEST_(UDUB);
	RIL_REQUEST_(LAST_CALL_FAIL_CAUSE);
	RIL_REQUEST_(SIGNAL_STRENGTH);
	RIL_REQUEST_(VOICE_REGISTRATION_STATE);
	RIL_REQUEST_(DATA_REGISTRATION_STATE);
	RIL_REQUEST_(OPERATOR);
	RIL_REQUEST_(RADIO_POWER);
	RIL_REQUEST_(DTMF);
	RIL_REQUEST_(SEND_SMS);
	RIL_REQUEST_(SEND_SMS_EXPECT_MORE);
	RIL_REQUEST_(SETUP_DATA_CALL);
	RIL_REQUEST_(SIM_IO);
	RIL_REQUEST_(SEND_USSD);
	RIL_REQUEST_(CANCEL_USSD);
	RIL_REQUEST_(GET_CLIR);
	RIL_REQUEST_(SET_CLIR);
	RIL_REQUEST_(QUERY_CALL_FORWARD_STATUS);
	RIL_REQUEST_(SET_CALL_FORWARD);
	RIL_REQUEST_(QUERY_CALL_WAITING);
	RIL_REQUEST_(SET_CALL_WAITING);
	RIL_REQUEST_(SMS_ACKNOWLEDGE);
	RIL_REQUEST_(GET_IMEI);
	RIL_REQUEST_(GET_IMEISV);
	RIL_REQUEST_(ANSWER);
	RIL_REQUEST_(DEACTIVATE_DATA_CALL);
	RIL_REQUEST_(QUERY_FACILITY_LOCK);
	RIL_REQUEST_(SET_FACILITY_LOCK);
	RIL_REQUEST_(CHANGE_BARRING_PASSWORD);
	RIL_REQUEST_(QUERY_NETWORK_SELECTION_MODE);
	RIL_REQUEST_(SET_NETWORK_SELECTION_AUTOMATIC);
	RIL_REQUEST_(SET_NETWORK_SELECTION_MANUAL);
	RIL_REQUEST_(QUERY_AVAILABLE_NETWORKS);
	RIL_REQUEST_(DTMF_START);
	RIL_REQUEST_(DTMF_STOP);
	RIL_REQUEST_(BASEBAND_VERSION);
	RIL_REQUEST_(SEPARATE_CONNECTION);
	RIL_REQUEST_(SET_MUTE);
	RIL_REQUEST_(GET_MUTE);
	RIL_REQUEST_(QUERY_CLIP);
	RIL_REQUEST_(LAST_DATA_CALL_FAIL_CAUSE);
	RIL_REQUEST_(DATA_CALL_LIST);
	RIL_REQUEST_(RESET_RADIO);
	RIL_REQUEST_(OEM_HOOK_RAW);
	RIL_REQUEST_(OEM_HOOK_STRINGS);
	RIL_REQUEST_(SCREEN_STATE);
	RIL_REQUEST_(SET_SUPP_SVC_NOTIFICATION);
	RIL_REQUEST_(WRITE_SMS_TO_SIM);
	RIL_REQUEST_(DELETE_SMS_ON_SIM);
	RIL_REQUEST_(SET_BAND_MODE);
	RIL_REQUEST_(QUERY_AVAILABLE_BAND_MODE);
	RIL_REQUEST_(STK_GET_PROFILE);
	RIL_REQUEST_(STK_SET_PROFILE);
	RIL_REQUEST_(STK_SEND_ENVELOPE_COMMAND);
	RIL_REQUEST_(STK_SEND_TERMINAL_RESPONSE);
	RIL_REQUEST_(STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM);
	RIL_REQUEST_(EXPLICIT_CALL_TRANSFER);
	RIL_REQUEST_(SET_PREFERRED_NETWORK_TYPE);
	RIL_REQUEST_(GET_PREFERRED_NETWORK_TYPE);
	RIL_REQUEST_(GET_NEIGHBORING_CELL_IDS);
	RIL_REQUEST_(SET_LOCATION_UPDATES);
	RIL_REQUEST_(CDMA_SET_SUBSCRIPTION_SOURCE);
	RIL_REQUEST_(CDMA_SET_ROAMING_PREFERENCE);
	RIL_REQUEST_(CDMA_QUERY_ROAMING_PREFERENCE);
	RIL_REQUEST_(SET_TTY_MODE);
	RIL_REQUEST_(QUERY_TTY_MODE);
	RIL_REQUEST_(CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE);
	RIL_REQUEST_(CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE);
	RIL_REQUEST_(CDMA_FLASH);
	RIL_REQUEST_(CDMA_BURST_DTMF);
	RIL_REQUEST_(CDMA_VALIDATE_AND_WRITE_AKEY);
	RIL_REQUEST_(CDMA_SEND_SMS);
	RIL_REQUEST_(CDMA_SMS_ACKNOWLEDGE);
	RIL_REQUEST_(GSM_GET_BROADCAST_SMS_CONFIG);
	RIL_REQUEST_(GSM_SET_BROADCAST_SMS_CONFIG);
	RIL_REQUEST_(GSM_SMS_BROADCAST_ACTIVATION);
	RIL_REQUEST_(CDMA_GET_BROADCAST_SMS_CONFIG);
	RIL_REQUEST_(CDMA_SET_BROADCAST_SMS_CONFIG);
	RIL_REQUEST_(CDMA_SMS_BROADCAST_ACTIVATION);
	RIL_REQUEST_(CDMA_SUBSCRIPTION);
	RIL_REQUEST_(CDMA_WRITE_SMS_TO_RUIM);
	RIL_REQUEST_(CDMA_DELETE_SMS_ON_RUIM);
	RIL_REQUEST_(DEVICE_IDENTITY);
	RIL_REQUEST_(EXIT_EMERGENCY_CALLBACK_MODE);
	RIL_REQUEST_(GET_SMSC_ADDRESS);
	RIL_REQUEST_(SET_SMSC_ADDRESS);
	RIL_REQUEST_(REPORT_SMS_MEMORY_STATUS);
	RIL_REQUEST_(REPORT_STK_SERVICE_IS_RUNNING);
	RIL_REQUEST_(CDMA_GET_SUBSCRIPTION_SOURCE);
	RIL_REQUEST_(ISIM_AUTHENTICATION);
	RIL_REQUEST_(ACKNOWLEDGE_INCOMING_GSM_SMS_WITH_PDU);
	RIL_REQUEST_(STK_SEND_ENVELOPE_WITH_STATUS);
	RIL_REQUEST_(VOICE_RADIO_TECH);
	RIL_REQUEST_(GET_CELL_INFO_LIST);
	RIL_REQUEST_(SET_UNSOL_CELL_INFO_LIST_RATE);
	RIL_REQUEST_(SET_INITIAL_ATTACH_APN);
	RIL_REQUEST_(IMS_REGISTRATION_STATE);
	RIL_REQUEST_(IMS_SEND_SMS);
	RIL_REQUEST_(SIM_TRANSMIT_APDU_BASIC);
	RIL_REQUEST_(SIM_OPEN_CHANNEL);
	RIL_REQUEST_(SIM_CLOSE_CHANNEL);
	RIL_REQUEST_(SIM_TRANSMIT_APDU_CHANNEL);
	RIL_REQUEST_(NV_READ_ITEM);
	RIL_REQUEST_(NV_WRITE_ITEM);
	RIL_REQUEST_(NV_WRITE_CDMA_PRL);
	RIL_REQUEST_(NV_RESET_CONFIG);
	RIL_REQUEST_(SET_UICC_SUBSCRIPTION);
	RIL_REQUEST_(ALLOW_DATA);
	RIL_REQUEST_(GET_HARDWARE_CONFIG);
	RIL_REQUEST_(SIM_AUTHENTICATION);
	RIL_REQUEST_(GET_DC_RT_INFO);
	RIL_REQUEST_(SET_DC_RT_INFO_RATE);
	RIL_REQUEST_(SET_DATA_PROFILE);
	RIL_REQUEST_(SHUTDOWN);
	RIL_REQUEST_(GET_RADIO_CAPABILITY);
	RIL_REQUEST_(SET_RADIO_CAPABILITY);
	default:
		snprintf(unknown, sizeof(unknown), "RIL_REQUEST_%d", request);
		return unknown;
	}
}

const char *ril_unsol_event_to_string(guint event)
{
#define RIL_UNSOL_(name) case RIL_UNSOL_##name: return #name
	static char unknown[24];
	switch (event) {
	RIL_UNSOL_(RESPONSE_RADIO_STATE_CHANGED);
	RIL_UNSOL_(RESPONSE_CALL_STATE_CHANGED);
	RIL_UNSOL_(RESPONSE_VOICE_NETWORK_STATE_CHANGED);
	RIL_UNSOL_(RESPONSE_NEW_SMS);
	RIL_UNSOL_(RESPONSE_NEW_SMS_STATUS_REPORT);
	RIL_UNSOL_(RESPONSE_NEW_SMS_ON_SIM);
	RIL_UNSOL_(ON_USSD);
	RIL_UNSOL_(ON_USSD_REQUEST);
	RIL_UNSOL_(NITZ_TIME_RECEIVED);
	RIL_UNSOL_(SIGNAL_STRENGTH);
	RIL_UNSOL_(DATA_CALL_LIST_CHANGED);
	RIL_UNSOL_(SUPP_SVC_NOTIFICATION);
	RIL_UNSOL_(STK_SESSION_END);
	RIL_UNSOL_(STK_PROACTIVE_COMMAND);
	RIL_UNSOL_(STK_EVENT_NOTIFY);
	RIL_UNSOL_(STK_CALL_SETUP);
	RIL_UNSOL_(SIM_SMS_STORAGE_FULL);
	RIL_UNSOL_(SIM_REFRESH);
	RIL_UNSOL_(CALL_RING);
	RIL_UNSOL_(RESPONSE_SIM_STATUS_CHANGED);
	RIL_UNSOL_(RESPONSE_CDMA_NEW_SMS);
	RIL_UNSOL_(RESPONSE_NEW_BROADCAST_SMS);
	RIL_UNSOL_(CDMA_RUIM_SMS_STORAGE_FULL);
	RIL_UNSOL_(RESTRICTED_STATE_CHANGED);
	RIL_UNSOL_(ENTER_EMERGENCY_CALLBACK_MODE);
	RIL_UNSOL_(CDMA_CALL_WAITING);
	RIL_UNSOL_(CDMA_OTA_PROVISION_STATUS);
	RIL_UNSOL_(CDMA_INFO_REC);
	RIL_UNSOL_(OEM_HOOK_RAW);
	RIL_UNSOL_(RINGBACK_TONE);
	RIL_UNSOL_(RESEND_INCALL_MUTE);
	RIL_UNSOL_(CDMA_SUBSCRIPTION_SOURCE_CHANGED);
	RIL_UNSOL_(CDMA_PRL_CHANGED);
	RIL_UNSOL_(EXIT_EMERGENCY_CALLBACK_MODE);
	RIL_UNSOL_(RIL_CONNECTED);
	RIL_UNSOL_(VOICE_RADIO_TECH_CHANGED);
	RIL_UNSOL_(CELL_INFO_LIST);
	RIL_UNSOL_(RESPONSE_IMS_NETWORK_STATE_CHANGED);
	RIL_UNSOL_(UICC_SUBSCRIPTION_STATUS_CHANGED);
	RIL_UNSOL_(SRVCC_STATE_NOTIFY);
	RIL_UNSOL_(HARDWARE_CONFIG_CHANGED);
	RIL_UNSOL_(DC_RT_INFO_CHANGED);
	RIL_UNSOL_(RADIO_CAPABILITY);
	RIL_UNSOL_(ON_SS);
	RIL_UNSOL_(STK_CC_ALPHA_NOTIFY);
	default:
		snprintf(unknown, sizeof(unknown), "RIL_UNSOL_%d", event);
		return unknown;
	}
}

const char *ril_radio_state_to_string(int radio_state)
{
#define RADIO_STATE_(name) case RADIO_STATE_##name: return #name
	static char unknown[16];
	switch (radio_state) {
	RADIO_STATE_(OFF);
	RADIO_STATE_(UNAVAILABLE);
	RADIO_STATE_(SIM_NOT_READY);
	RADIO_STATE_(SIM_LOCKED_OR_ABSENT);
	RADIO_STATE_(SIM_READY);
	RADIO_STATE_(RUIM_NOT_READY);
	RADIO_STATE_(RUIM_READY);
	RADIO_STATE_(RUIM_LOCKED_OR_ABSENT);
	RADIO_STATE_(NV_NOT_READY);
	RADIO_STATE_(NV_READY);
	RADIO_STATE_(ON);
	default:
		snprintf(unknown, sizeof(unknown), "%d (?)", radio_state);
		return unknown;
	}
}

int ril_address_family(const char *addr)
{
	if (strchr(addr, ':')) {
		return AF_INET6;
	} else if (strchr(addr, '.')) {
		return AF_INET;
	} else {
		return AF_UNSPEC;
	}
}

gboolean ril_util_parse_reg(const void *data, guint len,
						struct ril_reg_data *reg)
{
	GRilIoParser rilp;
	int nparams;
	gchar *sstatus = NULL, *slac = NULL, *sci = NULL;
	gchar *stech = NULL, *sreason = NULL, *smax = NULL;

	memset(reg, 0, sizeof(*reg));

	/* Size of response string array
	 *
	 * Should be:
	 *   >= 4 for VOICE_REG reply
	 *   >= 5 for DATA_REG reply
	 */
	grilio_parser_init(&rilp, data, len);
	if (!grilio_parser_get_int32(&rilp, &nparams) || nparams < 4) {
		DBG("broken response");
		return FALSE;
	}

	sstatus = grilio_parser_get_utf8(&rilp);
	if (!sstatus) {
		DBG("No sstatus value returned!");
		return FALSE;
	}

	slac = grilio_parser_get_utf8(&rilp);
	sci = grilio_parser_get_utf8(&rilp);
	stech = grilio_parser_get_utf8(&rilp);
	nparams -= 4;

	reg->ril_status = atoi(sstatus);
	if (reg->ril_status > 10) {
		reg->status = reg->ril_status - 10;
	} else {
		reg->status = reg->ril_status;
	}

	/* FIXME: need to review VOICE_REGISTRATION response
	 * as it returns ~15 parameters ( vs. 6 for DATA ).
	 *
	 * The first four parameters are the same for both
	 * responses ( although status includes values for
	 * emergency calls for VOICE response ).
	 *
	 * Parameters 5 & 6 have different meanings for
	 * voice & data response.
	 */
	if (nparams--) {
		/* TODO: different use for CDMA */
		sreason = grilio_parser_get_utf8(&rilp);
		if (nparams--) {
			/* TODO: different use for CDMA */
			smax = grilio_parser_get_utf8(&rilp);
			if (smax) {
				reg->max_calls = atoi(smax);
			}
		}
	}

	reg->lac = slac ? strtol(slac, NULL, 16) : -1;
	reg->ci = sci ? strtol(sci, NULL, 16) : -1;
	reg->access_tech = ril_parse_tech(stech, &reg->ril_tech);

	DBG("%s,%s,%s,%d,%s,%s,%s", registration_status_to_string(reg->status),
			slac, sci, reg->ril_tech,
			registration_tech_to_string(reg->access_tech),
			sreason, smax);

	g_free(sstatus);
	g_free(slac);
	g_free(sci);
	g_free(stech);
	g_free(sreason);
	g_free(smax);
	return TRUE;
}

/* Returns enum access_technology or -1 on failure. */
int ril_parse_tech(const char *stech, int *ril_tech)
{
	int access_tech = -1;
	int tech = -1;
	if (stech && stech[0]) {
		tech = atoi(stech);
		switch (tech) {
		case RADIO_TECH_GPRS:
		case RADIO_TECH_GSM:
			access_tech = ACCESS_TECHNOLOGY_GSM;
			break;
		case RADIO_TECH_EDGE:
			access_tech = ACCESS_TECHNOLOGY_GSM_EGPRS;
			break;
		case RADIO_TECH_UMTS:
			access_tech = ACCESS_TECHNOLOGY_UTRAN;
			break;
		case RADIO_TECH_HSDPA:
			access_tech = ACCESS_TECHNOLOGY_UTRAN_HSDPA;
			break;
		case RADIO_TECH_HSUPA:
			access_tech = ACCESS_TECHNOLOGY_UTRAN_HSUPA;
			break;
		case RADIO_TECH_HSPA:
		case RADIO_TECH_HSPAP:
			access_tech = ACCESS_TECHNOLOGY_UTRAN_HSDPA_HSUPA;
			break;
		case RADIO_TECH_LTE:
			access_tech = ACCESS_TECHNOLOGY_EUTRAN;
			break;
		default:
			DBG("Unknown RIL tech %s", stech);
			/* no break */
		case RADIO_TECH_IWLAN:
		case RADIO_TECH_UNKNOWN:
			tech = -1;
			break;
		}

	}
	if (ril_tech) {
		*ril_tech = tech;
	}
	return access_tech;
}

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 8
 * indent-tabs-mode: t
 * End:
 */
