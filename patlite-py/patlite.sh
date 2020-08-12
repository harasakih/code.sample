#!/bin/sh
#
# syslog Serverity に応じて、パトライトのライト、ブザーをコントロールする
#
#
#  Serverity         patlite
#                 R  Y  G   BUZZ     BG BY BR ZL ZS LG LY LR
#  0 : emerg   7  +- +- +-  Short    1  1  1  0  1  0  0  0
#  1 : alert   6  +- +- 0   Short    0  1  1  0  1  0  0  0
#  2 : crit    5  +- 0  +-  Short    1  0  1  0  1  0  0  0
#  3 : err     4  +- 0  0   Short    0  0  1  0  1  0  0  0
#  4 : warn    3  0  ++ ++  Long     0  0  0  1  0  1  1  0
#  5 : notice  2  0  ++ 0   (none)   0  0  0  0  0  0  1  0
#  6 : info    1  0  0  ++  (none)   0  0  0  0  0  1  0  0
#  7 : debug   0  0  0  0   (none)
#
usage() {
  echo "usage: `basename $0` SERVERITY [-dxxx] [-pxxx]"
  echo "  SERVERITY = emer|alert|crit|err|warn|notice|info|reset"
  echo "  -d Dest ip-address of patlite"
  echo "  -p Dest port of patlite"
  echo "  -h Host-IP:Host-port, Host is patlite-controller"
  [ "$1" == "" ] && exit 16
  exit $1
}

OPTS="d:p:h:"
OPT_D=0
OPT_P=0
OPT_H=0
do_getopt () {
	local OPTIND OPT OPTARG

	while getopts $OPTS OPT ; do
	case $OPT in
		d)
			OPT_D=1  &&  DEST=$OPTARG
			;;
		p)
			OPT_P=1  &&  PORT=$OPTARG
			;;
		h)
			OPT_H=1  &&  HOST=$OPTARG
			;;
		*)
			;;
	esac
	done
#	shift $((OPTIND - 1))
}

#############################
##	Init
##############################
##
DEST="10.0.150.74"
PORT="10000"
HOST="localhost:8080"
CURL_TIMEOUT=60                  # 60sec to timeout

  [ "$#" == "0" ] && usage 1
  SERV=$1
  shift 
  do_getopt $@

  case $SERV in
  "emer" )
    PARM="11101000"  ;;
  "alert" )
    PARM="01101000"  ;;
  "crit"  )
    PARM="10101000"  ;;
  "err"   )
    PARM="00101000"  ;;
  "warn"  )
#    PARM="00010110"  ;;
    PARM="00000110"  ;;
  "info"  )
    PARM="00000100"  ;;
  "reset" )
    PARM="00000000"  ;;
  *)
    usage 8
  esac

  STR="http://$HOST/patlite?p=$PARM"
  STR=$STR"&dest=$DEST"
  STR=$STR"&port=$PORT"
  echo $STR
  curl -m $CURL_TIMEOUT -X GET $STR

#                 R  Y  G   BUZZ     BG BY BR ZL ZS LG LY LR
#  0 : emerg   7  +- +- +-  Short    1  1  1  0  1  0  0  0
#  1 : alert   6  +- +- 0   Short    0  1  1  0  1  0  0  0
#  2 : crit    5  +- 0  +-  Short    1  0  1  0  1  0  0  0
#  3 : err     4  +- 0  0   Short    0  0  1  0  1  0  0  0
#  4 : warn    3  0  ++ ++  Long     0  0  0  1  0  1  1  0
#  5 : notice  2  0  ++ 0   (none)   0  0  0  0  0  0  1  0
#  6 : info    1  0  0  ++  (none)   0  0  0  0  0  1  0  0
#  7 : debug   0  0  0  0   (none)



