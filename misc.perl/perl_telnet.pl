#!/usr/bin/perl
#
#	$Id :$
#

use strict;
use Net::Telnet;

# 接続情報(環境に合わせて修正する)
my $host = '192.168.0.26';		# 接続ホスト名 or IPアドレス
my $user = 'vmware';			# ユーザ名
my $pass = 'vmware';			# パスワード

#my $prompt = '/\[.+\]\$ $/';	# プロンプト(正規表現)
my $prompt = '/.+\$ $/';		# 


my $telnet = new Net::Telnet(
	Timeout => 10,
	Prompt => $prompt,
);

# ホストに接続してログインする
$telnet->open($host);
$telnet->login($user, $pass);

# コマンドの実行
my @result;
@result = $telnet->cmd("pwd");
print "pwd>\n";
print @result;
print "<pwd\n";

# コマンドの実行
@result = $telnet->cmd("ls -ail");
print "ls -l>\n";
print @result;
print "<ls -ail\n";

# 接続の切断
$telnet->close;
