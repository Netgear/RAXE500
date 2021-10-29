<!--
 HTML part for Cpu Load Demo Tab

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

 $Id: cpuload_demo.asp 747363 2018-02-16 08:53:47Z $
-->

<!DOCTYPE html>

<html>
<head>
<meta charset="ISO-8859-1">
<link rel="stylesheet" href="cpuload_demo.css"/>
<script src="jquery-2.0.3.min.js"></script>
<script src="gauge2.min.js"></script>
<script src="cpuload_demo.js"></script>
<title>Wlan speed and cpu load</title>
</head>
<body>
	<div id="main_div">
		<table class="logotable">
			<tr>
				<td colspan="2" class="edge"><img border="0" src="blur_new.jpg" alt=""/></td>
			</tr>
			<tr>
				<td><img border="0" src="logo_new.gif" alt=""/></td>
				<td width="100%" valign="top">
					<br/>
					<span id="TextHeading">Wlan speed and cpu load</span><br/>
					<span id="TextHeadingDesc">In this page we will demonstrate
					wlan speed and cpu load</span>
				</td>
			</tr>
		</table>
	</div>
	<div class="leftmargin rightmargin">
	<div>
		<div style="float:left">
			<canvas id="canv-gauge-tx"></canvas>
			<canvas id="canv-gauge" class="leftmargin"></canvas>
			<canvas id="cpu-meter" class="leftmargin"></canvas>
		</div>
		<div style="float:rigth"/>
		<div style="clear:both"/>
	</div>
	<div class="topmargin" style="display:none">
		<button id="cfgbtn"class="actbtn" style="float:left">H/W Accel ON</button>
		<div id="status">Status : H/W Accel is OFF</div>
		<div style="clear:both"/>
	</div>
	</div>
</body>
</html>
