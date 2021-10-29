<!--
Copyright (C) 2020, Broadcom. All Rights Reserved.

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

<<Broadcom-WL-IPTag/Open:>>

$Id: security.asp,v 1.54 2011-01-11 18:43:43 $
-->

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html lang="en">
<head>
<title>Broadcom Home Gateway Reference Design: Security</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="style.css" media="screen">
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript">

function update_ui_ctrls_for_enterprise_security(wpa_state)
{
	document.forms[0].wl_radius_ipaddr.disabled = !wpa_state;
	document.forms[0].wl_radius_port.disabled = !wpa_state;
	document.forms[0].wl_radius_key.disabled = !wpa_state;
	document.forms[0].wl_preauth.disabled = !wpa_state;
	document.forms[0].wl_net_reauth.disabled = !wpa_state;
	document.forms[0].wl_akm_psk2.disabled = wpa_state;
	document.forms[0].wl_akm_sae.disabled = wpa_state;
	document.forms[0].wl_akm_dpp.disabled = wpa_state;
}

function update_ui_ctrls_for_psk_security(psk_state)
{
	document.forms[0].wl_wpa_psk.disabled = !psk_state;
	document.forms[0].wl_akm_wpa2.disabled = psk_state;
	document.forms[0].wl_akm_wpa3.disabled = psk_state;
	document.forms[0].wl_akm_wpa3_suite_b.disabled = psk_state;
	document.forms[0].wl_preauth.disabled = psk_state;
}

function update_ui_ctrl_for_6g(six_g_state)
{
	document.forms[0].wl_akm_owe.disabled = !six_g_state;
	document.forms[0].wl_akm_sae.disabled = six_g_state;
	document.forms[0].wl_akm_psk2.disabled = six_g_state;
	document.forms[0].wl_akm_dpp.disabled = six_g_state;
	document.forms[0].wl_akm_wpa2.disabled = six_g_state;
	document.forms[0].wl_akm_wpa3.disabled = six_g_state;
	document.forms[0].wl_akm_wpa3_suite_b.disabled = six_g_state;
	document.forms[0].wl_radius_ipaddr.disabled = six_g_state;
	document.forms[0].wl_radius_port.disabled = six_g_state;
	document.forms[0].wl_radius_key.disabled = six_g_state;
	document.forms[0].wl_preauth.disabled = six_g_state;
	document.forms[0].wl_net_reauth.disabled = six_g_state;
}

function wl_akm_change()
{
	var wpa = document.forms[0].wl_akm_wpa[document.forms[0].wl_akm_wpa.selectedIndex].value;
	var psk = document.forms[0].wl_akm_psk[document.forms[0].wl_akm_psk.selectedIndex].value;
	var wpa2 = document.forms[0].wl_akm_wpa2[document.forms[0].wl_akm_wpa2.selectedIndex].value;
	var psk2 = document.forms[0].wl_akm_psk2[document.forms[0].wl_akm_psk2.selectedIndex].value;
	var sae = document.forms[0].wl_akm_sae[document.forms[0].wl_akm_sae.selectedIndex].value;
	var wpa3 = document.forms[0].wl_akm_wpa3[document.forms[0].wl_akm_wpa3.selectedIndex].value;
	var owe = document.forms[0].wl_akm_owe[document.forms[0].wl_akm_owe.selectedIndex].value;
	var dpp = document.forms[0].wl_akm_dpp[document.forms[0].wl_akm_dpp.selectedIndex].value;
	var wpa3_suite_b = document.forms[0].wl_akm_wpa3_suite_b[document.forms[0].wl_akm_wpa3_suite_b.selectedIndex].value;
	var band = "<% wl_cur_phytype(); %>";
	var wl_ibss = <% wl_ibss_mode(); %>;
	var security = (wpa == "enabled" || wpa2 == "enabled" || wpa3 == "enabled" || psk == "enabled" ||
		psk2 == "enabled" || sae == "enabled" || owe == "enabled" || dpp == "enabled");
	var is_mfp_required;
	var mfp_options;

	/* Initially disable wpa, psk and passphrase fields */
	document.forms[0].wl_akm_wpa.disabled = 1;
	document.forms[0].wl_akm_psk.disabled = 1;
	document.forms[0].wl_wpa_psk.disabled = 1;

	/* enable/disable gtk and ptk rotation interval */
	document.forms[0].wl_wpa_gtk_rekey.disabled = 1;
	document.forms[0].wl_wpa_ptk_rekey.disabled = 1;
	if (wl_ibss != "1" && security == true) {
		document.forms[0].wl_wpa_gtk_rekey.disabled = 0;
		document.forms[0].wl_wpa_ptk_rekey.disabled = 0;
	}

	if (band == "Current: 6 GHz") {
		update_ui_ctrl_for_6g(1);
		if (owe == "disabled") {
			document.forms[0].wl_akm_sae.disabled = 0;
		}
		if (sae == "enabled") {
			document.forms[0].wl_akm_owe.disabled = 1;
			document.forms[0].wl_akm_sae.disabled = 0;
		}
		document.forms[0].wl_wpa_psk.disabled = 0;
		if (owe != "enabled" && sae != "enabled") {
			document.forms[0].wl_wpa_psk.disabled = 1;
		}
	} else {
		update_ui_ctrl_for_6g(0);

		/* enable/disable Pre-shared Key */
		if (psk2 == "enabled" || sae == "enabled" || dpp == "enabled") {
			update_ui_ctrls_for_psk_security(1);
			document.forms[0].wl_akm_psk.disabled = 0;

			/* psk needs to be disabled when sae or dpp is enabled */
			if (sae == "enabled" || dpp == "enabled") {
				document.forms[0].wl_akm_psk[document.forms[0].wl_akm_psk.selectedIndex].value = "disabled";
				document.forms[0].wl_akm_psk.disabled = 1;
			}
		} else  {
			update_ui_ctrls_for_psk_security(0);
		}

		if (wpa2 == "enabled" || wpa3 == "enabled" || wpa3_suite_b == "enabled") {
			/* enable radius options */
			update_ui_ctrls_for_enterprise_security(1);
			document.forms[0].wl_akm_wpa.disabled = 0;

			/* wpa1 needs to be disabled when wpa3_suite_b is enabled */
			if (wpa3_suite_b != "enabled") {
				document.forms[0].wl_akm_wpa3_suite_b[document.forms[0].wl_akm_wpa3_suite_b.selectedIndex].value = "disabled";
				document.forms[0].wl_akm_wpa3_suite_b.disabled = 1;
			} else {
				document.forms[0].wl_akm_wpa[document.forms[0].wl_akm_wpa.selectedIndex].value = "disabled";
				document.forms[0].wl_akm_wpa.disabled = 1;
				document.forms[0].wl_preauth[document.forms[0].wl_preauth.selectedIndex].value = "disabled";
				document.forms[0].wl_preauth.disabled = 1;
				document.forms[0].wl_akm_wpa2[document.forms[0].wl_akm_wpa2.selectedIndex].value = "disabled";
				document.forms[0].wl_akm_wpa2.disabled = 1;
				document.forms[0].wl_akm_wpa3[document.forms[0].wl_akm_wpa3.selectedIndex].value = "disabled";
				document.forms[0].wl_akm_wpa3.disabled = 1;
			}
		} else {
			update_ui_ctrls_for_enterprise_security(0);
		}
	}

	mfp_options = [{name:"Required", val:"2"}, {name:"Capable", val:"1"}, {name:"Off", val:"0"}];
	is_mfp_required = (sae == "enabled" && psk2 != "enabled") ||
		(wpa3_suite_b == "enabled" && wpa2 != "enabled" && wpa3 != "enabled") || (owe == "enabled") || (dpp == "enabled");
	if (is_mfp_required == true) {
		document.forms[0].wl_mfp.disabled = 0;
		document.forms[0].wl_mfp[0] = new Option(mfp_options[0].name, mfp_options[0].val);
		document.forms[0].wl_mfp[0].selected = true;
		document.forms[0].wl_mfp.length = 1;
	} else {
		var j = 0;
		/* update mfp value based on akm */
		for (var i = mfp_options.length - 1; i >= 0; i--) {
			document.forms[0].wl_mfp[j++] = new Option(mfp_options[i].name, mfp_options[i].val);
		}
		document.forms[0].wl_mfp.length = mfp_options.length;;
		document.forms[0].wl_mfp.disabled = 1;
		document.forms[0].wl_mfp[0].selected= true;
		if (wpa2 == "enabled" || wpa3 == "enabled" || psk2 == "enabled") {
			document.forms[0].wl_mfp.disabled = 0;
			document.forms[0].wl_mfp[1].selected = true;
		}
	}

	wl_crypto_update();
}

function wl_crypto_update()
{
	var i, cur, algos;
	var wl_ure = <% wl_ure_enabled(); %>;
	var wl_ibss = <% wl_ibss_mode(); %>;
	var wl_nmode = <% wl_nmode_enabled(); %>;

	/* Save current crypto algorithm */
	for (i = 0; i < document.forms[0].wl_crypto.length; i++) {
		if (document.forms[0].wl_crypto[i].selected) {
			cur = document.forms[0].wl_crypto[i].value;
			break;
		}
	}

	/* Define new crypto algorithms */
	if (<% wl_corerev(); %> >= 3) {
		if (wl_ibss == "1") {
			algos = ["AES"];
		} else {
			if ((document.forms[0].wl_akm_wpa.disabled == 0 &&
				document.forms[0].wl_akm_wpa[document.forms[0].wl_akm_wpa.selectedIndex].value == "enabled") ||
				(document.forms[0].wl_akm_psk.disabled == 0 &&
				document.forms[0].wl_akm_psk[document.forms[0].wl_akm_psk.selectedIndex].value == "enabled"))
				algos = ["AES", "TKIP+AES"];
			else if ((document.forms[0].wl_akm_wpa3_suite_b.disabled == 0 &&
                                document.forms[0].wl_akm_wpa3_suite_b[document.forms[0].wl_akm_wpa3_suite_b.selectedIndex].value == "enabled"))
				algos = ["Suite-B"];
			else
				algos = ["AES"];
		}
	}

	/* Reconstruct algorithm array from new crypto algorithms */
	document.forms[0].wl_crypto.length = algos.length;
	for (var i in algos) {
		document.forms[0].wl_crypto[i] = new Option(algos[i], algos[i].toLowerCase());
		document.forms[0].wl_crypto[i].value = algos[i].toLowerCase();
		if (document.forms[0].wl_crypto[i].value == cur)
			document.forms[0].wl_crypto[i].selected = true;
	}
}

function wl_security_update()
{
	wl_crypto_update();
	wl_akm_change();
}

function wpapsk_window()
{
	var psk_window = window.open("", "", "toolbar=no,scrollbars=yes,width=400,height=100");
	psk_window.document.write("The WPA passphrase is <% nvram_get("wl_wpa_psk"); %>");
	psk_window.document.close();
}

function pre_submit()
{
/*
*/
<% wps_security_pre_submit_display(); %>
/*
*/
	return true;
}

</script>
</head>

<body onLoad="wl_security_update();">
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>

<table id="page_header"  border="0" cellpadding="0" cellspacing="0" width="100%" bgcolor="#cc0000">
  <% asp_list(); %>
</table>

<table border="0" cellpadding="0" cellspacing="0" width="100%">
  <tr class="page_title">
    <td colspan="2" class="edge"><img border="0" src="blur_new.jpg" alt=""></td>
  </tr>
  <tr>
    <td class="page_title"><img border="0" src="logo_new.gif" alt=""></td>
    <td width="100%" valign="top">
	<br>
	<span class="title">SECURITY</span><br>
	<span class="subtitle">This page allows you to configure
	security for the wireless LAN interfaces.</span>
    </td>
  </tr>
</table>

<form method="post" action="security.asp">
<input type="hidden" name="page" value="security.asp">
<!--
-->
<input type="hidden" name="wl_wps_mode" value="<% nvram_get("wl_wps_mode"); %>">
<!--
-->
<p>
<table border="0" cellpadding="0" cellspacing="0">
  <tr>
    <th width="310"
	onMouseOver="return overlib('Selects which wireless interface to configure.', LEFT);"
	onMouseOut="return nd();">
	Wireless Interface:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td>
	<select name="wl_unit" onChange="submit();">
	  <% wl_list("INCLUDE_SSID" , "INCLUDE_VIFS"); %>
	</select>
    </td>
    <td>
	<button type="submit" name="action" value="Select">Select</button>
    </td>
  </tr>
</table>

<p>
<table border="0" cellpadding="0" cellspacing="0">
  <tr>
    <th width="310"
	onMouseOver="return overlib('Enables/Disables WPA Authenticated Key Management suite.', LEFT);"
	onMouseOut="return nd();">
	<input type="hidden" name="wl_akm" value="">
	WPA:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td>
	<select name="wl_akm_wpa" onChange="wl_akm_change();">
	  <option value="enabled" <% nvram_inlist("wl_akm", "wpa", "selected"); %>>Enabled</option>
	  <option value="disabled" <% nvram_invinlist("wl_akm", "wpa", "selected"); %>>Disabled</option>
	</select>
    </td>
  </tr>
  <tr>
    <th width="310"
	onMouseOver="return overlib('Enables/Disables WPA-PSK Authenticated Key Management suite.', LEFT);"
	onMouseOut="return nd();">
	WPA-PSK:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td>
	<select name="wl_akm_psk" onChange="wl_akm_change();">
	  <option value="enabled" <% nvram_inlist("wl_akm", "psk", "selected"); %>>Enabled</option>
	  <option value="disabled" <% nvram_invinlist("wl_akm", "psk", "selected"); %>>Disabled</option>
	</select>
    </td>
  </tr>
  <tr>
    <th width="310"
	onMouseOver="return overlib('Enables/Disables WPA2 Authenticated Key Management suite.', LEFT);"
	onMouseOut="return nd();">
	WPA2:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td>
	<select name="wl_akm_wpa2" onChange="wl_akm_change();">
	  <option value="enabled" <% nvram_inlist("wl_akm", "wpa2", "selected"); %>>Enabled</option>
	  <option value="disabled" <% nvram_invinlist("wl_akm", "wpa2", "selected"); %>>Disabled</option>
	</select>
    </td>
  </tr>
  <tr>
    <th width="310"
	onMouseOver="return overlib('Enables/Disables WPA2-PSK Authenticated Key Management suite.', LEFT);"
	onMouseOut="return nd();">
	WPA2-PSK:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td>
	<select name="wl_akm_psk2" onChange="wl_akm_change();">
	  <option value="enabled" <% nvram_inlist("wl_akm", "psk2", "selected"); %>>Enabled</option>
	  <option value="disabled" <% nvram_invinlist("wl_akm", "psk2", "selected"); %>>Disabled</option>
	</select>
    </td>
  </tr>
  <tr style="display:<% nvram_ternary("hapd_enable", "1", "table-row", "none"); %>;">
    <th width="310"
	onMouseOver="return overlib('Enables/Disables WPA3-SAE Authenticated Key Management suite.', LEFT);"
	onMouseOut="return nd();">
	WPA3-SAE:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td>
	<select name="wl_akm_sae" onChange="wl_akm_change();">
	  <option value="enabled" <% nvram_inlist("wl_akm", "sae", "selected"); %>>Enabled</option>
	  <option value="disabled" <% nvram_invinlist("wl_akm", "sae", "selected"); %>>Disabled</option>
	</select>
    </td>
  </tr>
  <tr>
    <th width="310"
	onMouseOver="return overlib('Enables/Disables WPA3 Authenticated Key Management suite.', LEFT);"
	onMouseOut="return nd();">
	WPA3:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td>
	<select name="wl_akm_wpa3" onChange="wl_akm_change();">
	  <option value="enabled" <% nvram_inlist("wl_akm", "wpa3", "selected"); %>>Enabled</option>
	  <option value="disabled" <% nvram_invinlist("wl_akm", "wpa3", "selected"); %>>Disabled</option>
	</select>
    </td>
  </tr>
  <tr>
    <th width="310"
	onMouseOver="return overlib('Enables/Disables OWE Authenticated Key Management suite.', LEFT);"
	onMouseOut="return nd();">
	OWE:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td>
	<select name="wl_akm_owe" onChange="wl_akm_change();">
	  <option value="enabled" <% nvram_inlist("wl_akm", "owe", "selected"); %>>Enabled</option>
	  <option value="disabled" <% nvram_invinlist("wl_akm", "owe", "selected"); %>>Disabled</option>
	</select>
    </td>
  </tr>
  <tr>
    <th width="310"
	onMouseOver="return overlib('Enables/Disables DPP Authenticated Key Management suite.', LEFT);"
	onMouseOut="return nd();">
	DPP:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td>
	<select name="wl_akm_dpp" onChange="wl_akm_change();">
	  <option value="enabled" <% nvram_inlist("wl_akm", "dpp", "selected"); %>>Enabled</option>
	  <option value="disabled" <% nvram_invinlist("wl_akm", "dpp", "selected"); %>>Disabled</option>
	</select>
    </td>
  </tr>
  <tr>
    <th width="310">WPA2 Preauthentication:&nbsp;&nbsp;</th>
    <td>&nbsp;&nbsp;</td>
    <td>
	<select name="wl_preauth">
	  <option value="disabled" <% nvram_match("wl_preauth", "0", "selected"); %>>Disabled</option>
	  <option value="enabled" <% nvram_invmatch("wl_preauth", "0", "selected"); %>>Enabled</option>
	</select>
    </td>
  </tr>
  <tr>
    <th width="310"
	onMouseOver="return overlib('Enables/Disables WPA3-SuiteB Authenticated Key Management suite.', LEFT);"
	onMouseOut="return nd();">
	WPA3-SuiteB:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td>
	<select name="wl_akm_wpa3_suite_b" onChange="wl_akm_change();">
	  <option value="enabled" <% nvram_inlist("wl_akm", "suite-b", "selected"); %>>Enabled</option>
	  <option value="disabled" <% nvram_invinlist("wl_akm", "suite-b", "selected"); %>>Disabled</option>
	</select>
    </td>
   </tr>
</table>

<p>
<table border="0" cellpadding="0" cellspacing="0">
  <tr>
    <th width="310"
	onMouseOver="return overlib('Selects the WPA data encryption algorithm.', LEFT);"
	onMouseOut="return nd();">
	<div id="wl_wpa_encrypt_div">
	WPA Encryption:&nbsp;&nbsp;
	</div>
    </th>
    <td>&nbsp;&nbsp;</td>
    <td>
	<select name="wl_crypto">
	  <option value="tkip" <% nvram_match("wl_crypto", "tkip", "selected"); %>>TKIP</option>
	  <option value="aes" <% nvram_match("wl_crypto", "aes", "selected"); %>>AES</option>
	  <option value="suite-b" <% nvram_match("wl_crypto", "suite-b", "selected"); %>>Suite-B</option>
	  <option value="tkip+aes" <% nvram_match("wl_crypto", "tkip+aes", "selected"); %>>TKIP+AES</option>
	</select>
    </td>
  </tr>
</table>

<p>
<table border="0" cellpadding="0" cellspacing="0">
  <tr>
    <th width="310"
	onMouseOver="return overlib('Sets the IP address of the RADIUS server to use for authentication and dynamic key derivation.', LEFT);"
	onMouseOut="return nd();">
	RADIUS Server:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td><input name="wl_radius_ipaddr" value="<% nvram_get("wl_radius_ipaddr"); %>" size="15" maxlength="15"></td>
  </tr>
  <tr>
    <th width="310"
	onMouseOver="return overlib('Sets the UDP port number of the RADIUS server. The port number is usually 1812 or 1645 and depends upon the server.', LEFT);"
	onMouseOut="return nd();">
	RADIUS Port:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td><input name="wl_radius_port" value="<% nvram_get("wl_radius_port"); %>" size="5" maxlength="5"></td>
  </tr>
  <tr>
    <th width="310"
	onMouseOver="return overlib('Sets the shared secret for the RADIUS connection.', LEFT);"
	onMouseOut="return nd();">
	RADIUS Key:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td><input name="wl_radius_key" value="<% nvram_get("wl_radius_key"); %>" type="password"></td>
  </tr>
</table>

<p>
<table border="0" cellpadding="0" cellspacing="0">
  <tr>
    <th width="310"
	onMouseOver="return overlib('Sets the WPA passphrase.', LEFT);"
	onMouseOut="return nd();">

	<div id="wl_wpa_psk_div">
	WPA passphrase:&nbsp;&nbsp;
	</div>
    </th>
    <td>&nbsp;&nbsp;</td>
    <td><input name="wl_wpa_psk" value="<% nvram_get("wl_wpa_psk"); %>" type="password"></td>
    <td>&nbsp;&nbsp;</td>
    <td> <A HREF="javascript:wpapsk_window()">Click here to display</A></td>
  </tr>
</table>

<!--
#ifdef MFP
-->
<p>
<table border="0" cellpadding="0" cellspacing="0">
  <tr>
    <th width="310">Protected Management Frames:&nbsp;&nbsp;</th>
    <td>&nbsp;&nbsp;</td>
    <td>
	<select name="wl_mfp">
	  <option value="0" <% nvram_match("wl_mfp", "0", "selected"); %>>Off</option>
	  <option value="1" <% nvram_match("wl_mfp", "1", "selected"); %>>Capable</option>
	  <option value="2" <% nvram_match("wl_mfp", "2", "selected"); %>>Required</option>
	</select>
    </td>
  </tr>
</table>
<!--
#endif // endif
-->
<p>
<table border="0" cellpadding="0" cellspacing="0">
  <tr>
    <th width="310"
	onMouseOver="return overlib('Sets the Network Key Rotation interval in seconds. Leave blank or set to zero to disable the rotation.', LEFT);"
	onMouseOut="return nd();">
	Network Key Rotation Interval:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td><input name="wl_wpa_gtk_rekey" value="<% nvram_get("wl_wpa_gtk_rekey"); %>" size="10" maxlength="10"></td>
  </tr>
<p>
<table border="0" cellpadding="0" cellspacing="0">
  <tr>
    <th width="310"
	onMouseOver="return overlib('Sets the Pairwise Key Rotation interval in seconds. Leave blank or set to zero to disable the rotation.', LEFT);"
	onMouseOut="return nd();">
	Pairwise Key Rotation Interval:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td><input name="wl_wpa_ptk_rekey" value="<% nvram_get("wl_wpa_ptk_rekey"); %>" size="10" maxlength="10"></td>
  </tr>
</table>

<p>
<table border="0" cellpadding="0" cellspacing="0">
  <tr>
    <th width="310"
	onMouseOver="return overlib('Sets the Network Re-authentication interval in seconds. Leave blank or set to zero to disable periodic network re-authentication.', LEFT);"
	onMouseOut="return nd();">
	Network Re-auth Interval:&nbsp;&nbsp;
    </th>
    <td>&nbsp;&nbsp;</td>
    <td><input name="wl_net_reauth" value="<% nvram_get("wl_net_reauth"); %>" size="10" maxlength="10"></td>
  </tr>
</table>
<p>
<table border="0" cellpadding="0" cellspacing="0">
    <tr>
      <td width="310"></td>
      <td>&nbsp;&nbsp;</td>
      <td>
	  <input type="submit" name="action" value="Apply" onClick="return pre_submit();">
	  <input type="reset" name="action" value="Cancel">
      </td>
    </tr>
</table>

</form>

<p class="label">&#169;2001-2016 Broadcom Limited. All rights reserved. 54g and XPress are trademarks of Broadcom Limited.</p>

</body>
</html>
