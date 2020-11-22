<HTML>
	<HEAD>
	<META HTTP-EQUIV="CONTENT-TYPE" CONTENT="text/html; charset=utf8">
	</HEAD>

<BODY>
<PRE>

<?php
require_once('./ipaddressc.php');

	if( isset($_GET['cmd'] )) { $cmd = $_GET['cmd']; }
	else { echo "cmd is null" . "\n" ; return null; }
	
	if( isset($_GET['ip'] )) { $cmd = $_GET['ip']; }
	else { echo "ip is null" . "\n" ; return null; }

	echo "cmd=" . $cmd " ip=" . $ip . "<br>\n";
	if(strcmp($cmd, "ipinfl") == 0) {
		list($net_addr, $net_maskl) = explode($ip, "/");
		echo "Network-address   :" . ip_nwaddrl($net_addr, $net_maskl) . "<BR>\n";
		echo "Broadcast-address :" . ip_broadaddrl($net_addr, $net_maskl) . "<BR>\n";
		echo "__EOD__" . "<BR>\n";
	}
	elseif(strcmp($cmd, "ipinfw") == 0) {
		list($net_addr, $net_wildmask) = explode($ip, "/");
		echo "Network-address   :" . ip_nwaddrw($net_addr, $net_wildmask) . "<BR>\n";
		echo "Broadcast-address :" . ip_broadaddrw($net_addr, $net_wildmask) . "<BR>\n";
		echo "__EOD__" . "<BR>\n";
	}
	elseif(strcmp($cmd, "ipinnetl") == 0) {
		list($net_addr, $net_maskl) = explode($ip, "/");
		if( ip_innetl($net_addr, $net_maskl) == True) {
			echo "TRUW" . "<BR>\n";
		} else {
			echo "FALSE" . "<BR>\n";
		}
	}
	elseif(strcmp($cmd, "ipinnetl") == 0) {
		list($net_addr, $net_wildmask) = explode($ip, "/");
		if( ip_innetl($net_addr, $net_wildmask) == True) {
			echo "TRUW" . "<BR>\n";
		} else {
			echo "FALSE" . "<BR>\n";
		}
	}
?>
</PRE>
</BODY>
</HTML>
