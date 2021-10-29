/*
 * Minimal debug/trace/assert driver definitions for
 * Broadcom 802.11 Networking Adapter.
 *
 * Copyright (C) 2020, Broadcom. All Rights Reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: wl_dbg.h 788459 2020-07-01 20:44:38Z $
 */

/* XXX, some old "wl msglevel" for PHY module has been moved to phy on 6/10/2009 "wl phymsglevel"
 * They are spare in TOT and can be reused if needed. see src/wl/phy/wlc_phy_int.h
 */

#ifndef _wl_dbg_h_
#define _wl_dbg_h_

#if defined(PKTC_TBL) && !defined(BCMDONGLEHOST)
#include <wlc_pub.h>
#include <wlc_dump.h>
#endif // endif

/* wl_msg_level is a bit vector with defs in wlioctl.h */
extern uint32 wl_msg_level;
extern uint32 wl_msg_level2;

#if !defined(BCMDONGLEHOST) && !defined(BCMDBG_EXCLUDE_HW_TIMESTAMP)
extern char* wlc_dbg_get_hw_timestamp(void);
#define WL_TIMESTAMP() do { if (wl_msg_level2 & WL_TIMESTAMP_VAL) {\
				printf(wlc_dbg_get_hw_timestamp()); }\
				} while (0)
#else
#define WL_TIMESTAMP()
#endif /* !BCMDONGLEHOST && !BCMDBG_EXCLUDE_HW_TIMESTAMP */

#ifdef ENABLE_CORECAPTURE
#define MAX_BACKTRACE_DEPTH 32
extern int wl_print_backtrace(const char * prefix, void * i_backtrace, int i_backtrace_depth);
#else
#define wl_print_backtrace(a, b, c)
#endif /* ENABLE_CORECAPTURE */

#define WIFICC_CAPTURE(_reason)
#define WIFICC_LOGDEBUGIF(_flags, _args)
#define WIFICC_LOGDEBUG(_args)

#define WL_PRINT(args)		do { WL_TIMESTAMP(); printf args; } while (0)

#define WL_SRSCAN(args)

#if defined(BCMCONDITIONAL_LOGGING)

/* Ideally this should be some include file that vendors can include to conditionalize logging */

/* DBGONLY() macro to reduce ifdefs in code for statements that are only needed when
 * BCMDBG is defined.
 */
#define DBGONLY(x) x

/* To disable a message completely ... until you need it again */
#define WL_NONE(args)
#define WL_ERROR(args)		do {if (wl_msg_level & WL_ERROR_VAL) WL_PRINT(args); \
					else WIFICC_LOGDEBUG(args); } while (0)
#define	WL_SCAN_ERROR(args)
#define	WL_IE_ERROR(args)
#define	WL_AMSDU_ERROR(args)
#define	WL_ASSOC_ERROR(args)
#define	KM_ERR(args)

#define WL_TRACE(args)
#define WL_PRHDRS_MSG(args)
#define WL_PRHDRS(i, p, f, t, r, l)
#define WL_PRPKT(m, b, n)
#define WL_INFORM(args)
#define WL_TMP(args)
#define WL_OID(args)
#define WL_RATE(args)		do {if (wl_msg_level & WL_RATE_VAL) WL_PRINT(args);} while (0)
#define WL_ASSOC(args)		do {if (wl_msg_level & WL_ASSOC_VAL) WL_PRINT(args); \
					else WIFICC_LOGDEBUG(args);} while (0)
#define WL_PRUSR(m, b, n)
#define WL_PS(args)		do {if (wl_msg_level & WL_PS_VAL) WL_PRINT(args);} while (0)

//#define WL_PORT(args)
#define WL_DUAL(args)
#define WL_REGULATORY(args)	do {if (wl_msg_level & WL_REGULATORY_VAL) WL_PRINT(args); \
					else WIFICC_LOGDEBUG(args);} while (0)

#define WL_MPC(args)
#define WL_APSTA(args)
#define WL_APSTA_BCN(args)
#define WL_APSTA_TX(args)
#define WL_APSTA_TSF(args)
#define WL_APSTA_BSSID(args)
#define WL_APSTA_WME(args)
#define WL_MBSS(args)
#define WL_MODE_SWITCH(args)
#define WL_PROTO(args)

#define	WL_CAC(args)		do {if (wl_msg_level & WL_CAC_VAL) WL_PRINT(args);} while (0)
#define WL_AMSDU(args)
#define WL_AMPDU(args)
#define WL_FFPLD(args)
#define WL_TWT(args)
#define WL_MCHAN(args)

#define WL_DFS(args)
#define WL_WOWL(args)
#define WL_ASSOC_OR_DPT(args)
#define WL_SCAN(args)		do {if (wl_msg_level2 & WL_SCAN_VAL) WL_PRINT(args);} while (0)
#define WL_COEX(args)
#define WL_RTDC(w, s, i, j)
#define WL_RTDC2(w, s, i, j)
#define WL_CHANINT(args)
#define WL_BTA(args)
#define WL_P2P(args)
#define WL_ITFR(args)
#define WL_TDLS(args)
#define WL_MCNX(args)
#define WL_PROT(args)
#define WL_PSTA(args)
#define WL_WFDS(m, b, n)
#define WL_TRF_MGMT(args)
#define WL_MAC(args)
#define WL_L2FILTER(args)
#define WL_MQ(args)
#define WL_TXBF(args)
#define WL_MUMIMO(args)
#define WL_MUTX(args)
#define WL_P2PO(args)
#define WL_ROAM(args)
#define WL_WNM(args)
#define WL_NAT(args)
#define WL_ULO(args)

#ifdef WLMSG_NATOE
#define WL_NAT(args)		do {if (wl_msg_level2 & WL_NATOE_VAL) WL_PRINT(args);} while (0)
#else
#define WL_NAT(args)
#endif // endif

#define WL_PFN_ERROR(args)

#define WL_AMPDU_UPDN(args)
#define WL_AMPDU_RX(args)
#define WL_AMPDU_ERR(args)
#define WL_AMPDU_TX(args)
#define WL_AMPDU_CTL(args)
#define WL_AMPDU_HW(args)
#define WL_AMPDU_HWTXS(args)
#define WL_AMPDU_HWDBG(args)
#define WL_AMPDU_STAT(args)
#define WL_AMPDU_ERR_ON()	0
#define WL_AMPDU_HW_ON()	0
#define WL_AMPDU_HWTXS_ON()	0

#define WL_APSTA_UPDN(args)
#define WL_APSTA_RX(args)
#define WL_APSTA_RX_ON()	0
#define WL_WSEC(args)
#define WL_WSEC_DUMP(args)
#define WL_PCIE(args)
#define	WL_AIRIQ(args)
#define WL_TSLOG(w, s, i, j)
#define WL_FBT(args)
#define WL_MBO_DBG(args)
#define WL_BAM_ERR(args)
#define WL_ADPS(args)
#define WL_OCE_DBG(args)
#define WL_OCE_ERR(args)
#define WL_OCE_INFO(args)

#define WL_ERROR_ON()		(wl_msg_level & WL_ERROR_VAL)
#define WL_TRACE_ON()		0
#define WL_PRHDRS_ON()		0
#define WL_PRPKT_ON()		0
#define WL_INFORM_ON()		0
#define WL_TMP_ON()		0
#define WL_OID_ON()		0
#define WL_RATE_ON()		(wl_msg_level & WL_RATE_VAL)
#define WL_ASSOC_ON()		(wl_msg_level & WL_ASSOC_VAL)
#define WL_PRUSR_ON()		0
#define WL_PS_ON()		(wl_msg_level & WL_PS_VAL)
#define WL_WSEC_ON()		0
#define WL_WSEC_DUMP_ON()	0
#define WL_MPC_ON()		0
#define WL_REGULATORY_ON()	(wl_msg_level & WL_REGULATORY_VAL)
#define WL_APSTA_ON()		0
#define WL_DFS_ON()		0
#define WL_MBSS_ON()		0
#define WL_CAC_ON()		(wl_msg_level & WL_CAC_VAL)
#define WL_AMPDU_ON()		0
#define WL_TWT_ON()		0
#define WL_WOWL_ON()		0
#define WL_SCAN_ON()		(wl_msg_level2 & WL_SCAN_VAL)
#define WL_BTA_ON()		0
#define WL_P2P_ON()		0
#define WL_ITFR_ON()		0
#define WL_MCHAN_ON()		0
#define WL_TDLS_ON()		0
#define WL_MCNX_ON()		0
#define WL_PROT_ON()		0
#define WL_PSTA_ON()		0
#define WL_TRF_MGMT_ON()	0
#define WL_MAC_ON()		0
#define WL_LPC_ON()		0
#define WL_L2FILTER_ON()	0
#define WL_TXBF_ON()		0
#define WL_P2PO_ON()		0
#define WL_TSLOG_ON()		0
#define WL_WNM_ON()		0
#define WL_PCIE_ON()		0
#define WL_MUMIMO_ON()		0
#define WL_MUTX_ON()		0
#define WL_AIRIQ_ON()		0
#define WL_MBO_DBG_ON()		0
#define WL_ADPS_ON()		0
#define WL_OCE_DBG_ON()		0

#else /* !BCMDBG */

/* DBGONLY() macro to reduce ifdefs in code for statements that are only needed when
 * BCMDBG is defined.
 */
#define DBGONLY(x)

/* To disable a message completely ... until you need it again */
#define WL_NONE(args)

#define	WL_AIRIQ(args)
#define WL_AIRIQ_ON()		0

#define	WL_ERROR(args)

#define  KM_ERR(args)

#define	WL_AMPDU_ERR(args)

#define	WL_TRACE(args)
#ifndef LINUX_POSTMOGRIFY_REMOVAL
#ifdef WLMSG_PRHDRS
#define	WL_PRHDRS_MSG(args)		WL_PRINT(args)
#define WL_PRHDRS(i, p, f, t, r, l)	wlc_print_hdrs(i, p, f, t, r, l)
#else
#define	WL_PRHDRS_MSG(args)
#define	WL_PRHDRS(i, p, f, t, r, l)
#endif // endif
#ifdef WLMSG_PRPKT
#define	WL_PRPKT(m, b, n)	prhex(m, b, n)
#else
#define	WL_PRPKT(m, b, n)
#endif // endif
#ifdef WLMSG_INFORM
#define	WL_INFORM(args)		WL_PRINT(args)
#else
#define	WL_INFORM(args)
#endif // endif
#define	WL_TMP(args)
#ifdef WLMSG_OID
#define WL_OID(args)		WL_PRINT(args)
#else
#define WL_OID(args)
#endif // endif
#define	WL_RATE(args)

#define	  WL_IE_ERROR(args)

#ifdef WLMSG_ASSOC
#define WL_ASSOC(args)		WL_PRINT(args)
#else
#define	WL_ASSOC(args)
#endif /* WLMSG_ASSOC */

#define	WL_ASSOC_ERROR(args)

#define	WL_SCAN_ERROR(args)

#define	WL_PRUSR(m, b, n)

#ifdef WLMSG_PS
#define WL_PS(args)		WL_PRINT(args)
#else
#define WL_PS(args)
#endif /* WLMSG_PS */

#define	WL_AMSDU_ERROR(args)

#ifdef WLMSG_ROAM
#define WL_ROAM(args)		WL_PRINT(args)
#else
#define WL_ROAM(args)
#endif /* WLMSG_ROAM */

//#define WL_PORT(args)
#define WL_DUAL(args)
#define WL_REGULATORY(args)
#define WL_TAF(args)

#ifdef WLMSG_MPC
#define WL_MPC(args)		WL_PRINT(args)
#else
#define WL_MPC(args)
#endif /* WLMSG_MPC */

#define WL_APSTA(args)
#define WL_APSTA_BCN(args)
#define WL_APSTA_TX(args)
#define WL_APSTA_TSF(args)
#define WL_APSTA_BSSID(args)
#define WL_APSTA_WME(args)
#define WL_MBSS(args)
#define WL_MODE_SWITCH(args)
#define	WL_PROTO(args)

#define	WL_CAC(args)
#define WL_AMSDU(args)
#define WL_AMPDU(args)
#define WL_FFPLD(args)
#define WL_TWT(args)
#define WL_MCHAN(args)
#define WL_BCNTRIM_DBG(args)

/* Define WLMSG_DFS automatically for WLTEST builds */
#if defined(WLTEST)
#ifndef WLMSG_DFS
#define WLMSG_DFS
#endif // endif
#endif /* WLTEST */

#ifdef WLMSG_DFS
#define WL_DFS(args)		do {if (wl_msg_level & WL_DFS_VAL) WL_PRINT(args);} while (0)
#else /* WLMSG_DFS */
#define WL_DFS(args)
#endif /* WLMSG_DFS */
#define WL_WOWL(args)

#ifdef WLMSG_SCAN
#define WL_SCAN(args)		WL_PRINT(args)
#else
#define WL_SCAN(args)
#endif /* WLMSG_SCAN */

#define	WL_COEX(args)
#define WL_RTDC(w, s, i, j)
#define WL_RTDC2(w, s, i, j)
#define WL_CHANINT(args)
#ifdef WLMSG_BTA
#define WL_BTA(args)		WL_PRINT(args)
#else
#define WL_BTA(args)
#endif // endif
#define WL_WMF(args)
#define WL_P2P(args)
#define WL_ITFR(args)
#define WL_TDLS(args)

#ifdef WLMSG_MCNX
#define WL_MCNX(args)		WL_PRINT(args)
#else
#define WL_MCNX(args)
#endif /* WLMSG_MCNX */

#define WL_PROT(args)
#define WL_PSTA(args)
#define WL_TBTT(args)
#define WL_TRF_MGMT(args)
#define WL_MAC(args)
#define WL_L2FILTER(args)
#define WL_MQ(args)
#define WL_P2PO(args)
#define WL_WNM(args)
#define WL_TXBF(args)
#define WL_TSLOG(w, s, i, j)
#define WL_FBT(args)
#define WL_MUMIMO(args)
#define WL_MUTX(args)
#define WL_RRM(args)
#define WL_RRM_HEX(m, b, n)
#define WL_SWDIV(args)
#define WL_ADPS(args)
#define WL_ULO(args)

#define WL_ERROR_ON()		0
#define WL_TRACE_ON()		0
#ifdef WLMSG_PRHDRS
#define WL_PRHDRS_ON()		1
#else
#define WL_PRHDRS_ON()		0
#endif // endif
#ifdef WLMSG_PRPKT
#define WL_PRPKT_ON()		1
#else
#define WL_PRPKT_ON()		0
#endif // endif
#ifdef WLMSG_INFORM
#define WL_INFORM_ON()		1
#else
#define WL_INFORM_ON()		0
#endif // endif
#ifdef WLMSG_OID
#define WL_OID_ON()		1
#else
#define WL_OID_ON()		0
#endif // endif
#define WL_TMP_ON()		0
#define WL_RATE_ON()		0
#ifdef WLMSG_ASSOC
#define WL_ASSOC_ON()		1
#else
#define WL_ASSOC_ON()		0
#endif // endif

#ifdef WLMSG_WSEC
#define WL_WSEC_ON()		1
#define WL_WSEC_DUMP_ON()	1
#else
#define WL_WSEC_ON()		0
#define WL_WSEC_DUMP_ON()	0
#endif // endif
#ifdef WLMSG_MPC
#define WL_MPC_ON()		1
#else
#define WL_MPC_ON()		0
#endif // endif
#define WL_REGULATORY_ON()	0

#define WL_APSTA_ON()		0
#define WL_MBSS_ON()		0
#define WL_MODE_SWITCH_ON()	0
#ifdef WLMSG_DFS
#define WL_DFS_ON()		1
#else /* WLMSG_DFS */
#define WL_DFS_ON()		0
#endif /* WLMSG_DFS */
#define WL_TWT_ON()		0
#ifdef WLMSG_SCAN
#define WL_SCAN_ON()		1
#else
#define WL_SCAN_ON()		0
#endif // endif
#ifdef WLMSG_BTA
#define WL_BTA_ON()		1
#else
#define WL_BTA_ON()		0
#endif // endif
#define WL_WMF_ON()		0
#define WL_P2P_ON()		0
#define WL_MCHAN_ON()		0
#define WL_TDLS_ON()		0
#define WL_MCNX_ON()		0
#define WL_PROT_ON()		0
#define WL_TBTT_ON()		0
#define WL_PWRSEL_ON()		0
#define WL_L2FILTER_ON()	0
#define WL_MAC_ON()		0
#define WL_MQ_ON()		0
#define WL_P2PO_ON()		0
#define WL_TXBF_ON()		0
#define WL_TSLOG_ON()		0
#define WL_MUMIMO_ON()		0
#define WL_MUTX_ON()		0
#define WL_SWDIV_ON()		0
#define WL_ULMU_ON()		0

#define WL_AMPDU_UPDN(args)
#define WL_AMPDU_RX(args)
#define WL_AMPDU_TX(args)
#define WL_AMPDU_CTL(args)
#define WL_AMPDU_HW(args)
#define WL_AMPDU_HWTXS(args)
#define WL_AMPDU_HWDBG(args)
#define WL_AMPDU_STAT(args)
#define WL_AMPDU_ERR_ON()	0
#define WL_AMPDU_HW_ON()	0
#define WL_AMPDU_HWTXS_ON()	0

#define WL_WNM_ON()		0
#ifdef WLMSG_MBO
#define WL_MBO_DBG_ON()		1
#else
#define WL_MBO_DBG_ON()		0
#endif /* WLMSG_MBO */
#define WL_ADPS_ON()		0
#ifdef WLMSG_OCE
#define WL_OCE_DBG_ON()		1
#else
#define WL_OCE_DBG_ON()		0
#endif /* WLMSG_OCE */
#endif /* LINUX_POSTMOGRIFY_REMOVAL */
#define WL_APSTA_UPDN(args)
#define WL_APSTA_RX(args)
#define WL_APSTA_RX_ON()	0

#ifdef WLMSG_WSEC
#define WL_WSEC(args)		WL_PRINT(args)
#define WL_WSEC_DUMP(args)	WL_PRINT(args)
#else
#define WL_WSEC(args)
#define WL_WSEC_DUMP(args)
#endif /* WLMSG_WSEC */

#ifdef WLMSG_MBO
#define WL_MBO_DBG(args)	WL_PRINT(args)
#define WL_MBO_INFO(args)	WL_PRINT(args)
#else
#define	WL_MBO_DBG(args)
#define	WL_MBO_INFO(args)
#endif /* WLMSG_MBO */

#define WL_MBO_ERR(args)

#ifdef WLMSG_OCE
#define WL_OCE_DBG(args)	WL_PRINT(args)
#define WL_OCE_INFO(args)	WL_PRINT(args)
#else
#define	WL_OCE_DBG(args)
#define	WL_OCE_INFO(args)
#endif /* WLMSG_OCE */

#define WL_OCE_ERR(args)

#define WL_PCIE(args)		do {if (wl_msg_level2 & WL_PCIE_VAL) WL_PRINT(args);} while (0)
#define WL_PCIE_ON()		(wl_msg_level2 & WL_PCIE_VAL)
#define WL_PFN(args)		do {if (wl_msg_level & WL_PFN_VAL) WL_PRINT(args);} while (0)
#define WL_PFN_ON()		(wl_msg_level & WL_PFN_VAL)
#endif // endif

#define WL_EAP_TRC_ERROR(args)
#define WL_EAP_TRC_INFO(args)
#define WL_EAP_TRC_SCAN(args)
#define WL_EAP_TRC_SCAN_DBG(args)
#define WL_EAP_TRC_INTR(args)
#define WL_EAP_TRC_RM(args)
#define WL_EAP_TRC_AMPDU(args)
#define WL_EAP_TRC_AMSDU(args)
#define WL_EAP_TRC_RXTXUTIL_MSG(args)
#define WL_EAP_TRC_DTIM(args)
#define WL_EAP_TRC_EVENT(args)

#ifdef WLMSG_BAM
#define WL_BAM_ERR(args)	WL_PRINT(args)
#endif /* WLMSG_BAM */

#define DBGERRONLY(x)

extern uint32 wl_msg_level;
extern uint32 wl_msg_level2;

/* Customers ask for more visibility when dealing with association problems, even in 'production'
 * firmware builds. Enabling WLMSG_ASSOC consumes approx 42KB of 4366c0 dongle RAM. As a lightweight
 * alternative, WLMSG_ASSOC_LT consumes 1KB on 4366c0. It does not generate all messages that its
 * full-blown sibling features, but the essential ones.
 */
#ifndef WLMSG_ASSOC_LT
#define WL_ASSOC_LT  WL_ASSOC
#else
#define WL_ASSOC_LT(args)  do {if (wl_msg_level & WL_ASSOC_VAL) WL_PRINT(args);} while (0)
#endif /* WLMSG_ASSOC_LT */

/* even in non-BCMDBG NIC builds, logging of iovars should be available */
#if !defined(DONGLEBUILD)
#define WL_G_IOV_ON()		(wl_msg_level & WL_G_IOV_VAL)
#define WL_S_IOV_ON()		(wl_msg_level2 & WL_S_IOV_VAL)
#else /* For full dongle, DHD prints messages instead of firmware */
#define WL_G_IOV_ON()		0
#define WL_S_IOV_ON()		0
#endif /* DONGLEBUILD */

#endif /* _wl_dbg_h_ */
