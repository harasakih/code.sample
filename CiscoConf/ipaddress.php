<?php
/*
  IP addressを操作するphp
*/
function ip_nwaddrl($net, $maskl)
/* NWアドレスを返却
$net str:	'192.168.10.20'
$maskl str:	'24'
return str: '192.168.10.0'
*/
{
	$inwmask = (0xffffffff >> (32 - $maskl)) << (32 - $maskl);
	$inw_address = ip2long($net) & $inwmask;
	return long2ip($inw_address);
}
function ip_nwaddrw($net, $wildmask)
/* NWアドレスを返却
$net str:	'192.168.10.20'
$wildmask str:	'0.0.0.255'
return str: '192.168.10.0'
*/
{
	$inwmask = ~ ip2long($wildmask);	// wildmaskのビット反転
	$inw_address = ip2long($net) & $inwmask;
	return long2ip($inw_address);
}
function ip_broadaddrl($net, $maskl)
/* Broadcastアドレスを返却
$net str :	'192.168.10.20'
$maskl str:	'24'
return str:	'192.168.10.255'
*/
{
	$ibrmask = (0xffffffff << $maskl) >> $maskl; 
	$inwaddr = ip2long(ip_nwaddrl($net, $maskl));
	$ibroadaddr = $inwaddr | $ibrmask;
	return long2ip($ibroadaddr);
}
function ip_broadaddrw($net, $wildmask)
/* Broadcastアドレスを返却
$net str :	'192.168.10.20'
$maskl str:	'0.0.0.255'
return str:	'192.168.10.255'
*/
{
    $ibrmask = ip2long($wildmask);
	$inwaddr = ip2long(ip_nwaddrw($net, $wildmask));
	$ibroadaddr = $inwaddr | $ibrmask;
	return long2ip($ibroadaddr);
}



function is_ipinnetl($ip, $net)
/* ipがnetに含まれるか
$ip str:	'192.168.0.1'
$net str:	'192.168.0.1/24'
RET bool: True / False
*/
{
	list($net_addr, $net_maskl) = explode('/', $net);
	$iip = ip_nwaddrl($ip, $net_maskl);
	$inet = ip_nwaddrl($net_addr, $net_maskl);
	if( $iip == $inet )
	{
		return True;
	} else {
		return False;
	}
}

function is_ipinnetw($ip, $net)
/*
$ip str:	'192.168.0.1'
$net str:	'192.168.0.1/0.0.0.255'
RET bool: True / False
*/
{
	list($net_addr, $net_wildmask) = explode('/', $net);
	$iip = ip2long($ip) & (~ ip2long($net_wildmask));
	$inet = ip2long($net_addr) & (~ ip2long($net_wildmask));

	if( $iip == $inet )
	{
		return True;
	} else {
		return False;
	}
}


function wild2maskl($wild)
/*
$wild str: 0.0.0.255
*/
{
	$oct = explode('.', $wild);
	$rtn = 0;
	for($i = 0; $i < 4; $i++) {
		$l = -1;
		switch((int) $oct[$i]) {
        case 0:
            $l = 0;
			break;
        case 1:
            $l = 1;
			break;
        case 3:
            $l = 2;
			break;
        case 7:
            $l = 3;
			break;
        case 15:
            $l = 4;
			break;
        case 31:
            $l = 5;
			break;
        case 63:
            $l = 6;
			break;
        case 127:
            $l = 7;
			break;
        case 255:
            $l = 8;
			break;
		default:
			$l = -1;
			break;
		}
		if( $l < 0 ) {
			return -1;
		} else {
			$rtn += $l;
		}
	}
  	return 32 - $rtn;
}
?>
