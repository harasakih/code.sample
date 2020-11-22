<?php
require('./ipaddress.php');

$ip = '192.168.0.1'; $netl = '192.168.0.1/24'; $netw = '192.168.0.1/0.0.0.255';
echo $ip, "/", $netl, ">", is_ipinnetl($ip, $netl), "\n";
echo $ip, "/", $netw, ">", is_ipinnetw($ip, $netw), "\n";
$ip = '192.168.0.1'; $netl = '192.168.0.1/32';$netw = '192.168.0.1/0.0.0.0';
echo $ip, "/", $netl, ">", is_ipinnetl($ip, $netl), "\n";
echo $ip, "/", $netw, ">", is_ipinnetw($ip, $netw), "\n";
$ip = '192.168.0.1'; $netl = '192.168.0.1/0';$netw = '192.168.0.1/255.255.255.255';
echo $ip, "/", $netl, ">", is_ipinnetl($ip, $netl), "\n";
echo $ip, "/", $netw, ">", is_ipinnetw($ip, $netw), "\n";
$ip = '192.168.1.1'; $netl = '192.168.0.1/24';$netw = '192.168.0.1/0.0.0.255';
echo $ip, "/", $netl, ">", is_ipinnetl($ip, $netl), "\n";
echo $ip, "/", $netw, ">", is_ipinnetw($ip, $netw), "\n";

$ip = '192.168.255.1'; $maskl = '23'; $wildmask = '0.0.1.255';
echo $ip, "/", $maskl, ">", ip_nwaddrl($ip, $maskl), "/", ip_broadaddrl($ip, $maskl), "\n";
echo $ip, "/", $wildmask, ">", ip_nwaddrw($ip, $wildmask), "/", ip_broadaddrw($ip, $wildmask), "\n";

$ip = '192.168.255.1'; $maskl = '25'; $wildmask = '0.0.0.127';
echo $ip, "/", $maskl, ">", ip_nwaddrl($ip, $maskl), "/", ip_broadaddrl($ip, $maskl), "\n";
echo $ip, "/", $wildmask, ">", ip_nwaddrw($ip, $wildmask), "/", ip_broadaddrw($ip, $wildmask), "\n";

$ip = '192.168.255.1'; $maskl = '32'; $wildmask = '0.0.0.0';
echo $ip, "/", $maskl, ">", ip_nwaddrl($ip, $maskl), "/", ip_broadaddrl($ip, $maskl), "\n";
echo $ip, "/", $wildmask, ">", ip_nwaddrw($ip, $wildmask), "/", ip_broadaddrw($ip, $wildmask), "\n";

$ip = '192.168.255.1'; $maskl = '0'; $wildmask = '255.255.255.255';
echo $ip, "/", $maskl, ">", ip_nwaddrl($ip, $maskl), "/", ip_broadaddrl($ip, $maskl), "\n";
echo $ip, "/", $wildmask, ">", ip_nwaddrw($ip, $wildmask), "/", ip_broadaddrw($ip, $wildmask), "\n";
?>
