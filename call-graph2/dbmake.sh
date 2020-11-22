#!/bin/sh
#
#   sqlite3のDB,TABLEを作成し、CSVからデータをロードする
#

do_usage() {
    echo "usage : `basename $0` DB-NAME EDGE.TSV NODE.TSV"
    [ "$1" != "" -a "$1" != "0" ] && exit $1
}
do_tablemake() {
    # table EDGE,NODEを作成する
    local DBFILE
    local SQL
    local TEMPSQL

    [ $# != 2 ] && exit 101
    DBFILE=$1
    SQL=$2
    TEMPSQL=`mktemp sqlite3.XXXXXX`

# make sql
cat << _EOT_ > $TEMPSQL
CREATE TABLE EDGE (
PARENT  varchar(32),
CHILD   varchar(32)
) ;
CREATE INDEX IDX_EDGE1 on EDGE (PARENT);
CREATE INDEX IDX_EDGE2 on EDGE (CHILD);

CREATE TABLE NODE (
KEY    varchar(64),
ITEM1  varchar(64),
ITEM2  varchar(64)
) ;
CREATE INDEX IDX_NODE1 on NODE (KEY);
.exit
_EOT_

# exec sql
    sqlite3 $DBFILE < $TEMPSQL
    cp $TEMPSQL $SQL
    rm $TEMPSQL
}

do_csvload() {
    local CSVFILE
    local TABLE
    local DBFILE
    local SQL
    local TEMPSQL

    [ $# != 4 ] && exit 102
    CSVFILE=$1
    TABLE=$2
    DBFILE=$3
    SQL=$4
    TEMPSQL=`mktemp sqlite3.XXXXXX`

# make sql
cat << _EOT_ > $TEMPSQL
.separator "\t"
.import $CSVFILE $TABLE
.exit
_EOT_

# exec sql
    sqlite3 $DBFILE < $TEMPSQL
    cp $TEMPSQL $SQL
    rm $TEMPSQL
}

#
#   main
#
[ $# -ne 3 ] && echo "Not correct ARG" && do_usage 1

DBFILE=$1                   # sqlite3-dbfile
EDGEFILE=$2                 # edge data tsv
NODEFILE=$3                 # node data tsv
SQL_MKTBL=mktbl.sql         # temp sql for create table
SQL_LDEDGE=load_edge.sql    # temp sql for import edge
SQL_LDNODE=load_node.sql    # temp sql for import node

[ -f $DBFILE ] && echo "delete DBFILE" && rm -i $DBFILE
[ ! -e $EDGEFILE ] && echo "File not found $EDGEFILE" && do_usage 2
[ ! -e $NODEFILE ] && echo "File not found $NODEFILE" && do_usage 3

# TABLE作成SQL
do_tablemake $DBFILE $SQL_MKTBL

# EDGE-FILEの整形
TEMPEDGE=`mktemp edge.XXXXXX`
cat $EDGEFILE | grep -v -e "^#" | grep -v -e "^[ \t]*$" > $TEMPEDGE
do_csvload $TEMPEDGE EDGE $DBFILE $SQL_LDEDGE

# NODE-FILEの整形
TEMPNODE=`mktemp node.XXXXXX`
cat $NODEFILE | grep -v -e "^#" | grep -v -e "^[ \t]*$" > $TEMPNODE
do_csvload $TEMPNODE NODE $DBFILE $SQL_LDNODE

rm $SQL_MKTBL $SQL_LDEDGE $SQL_LDNODE
rm $TEMPEDGE $TEMPNODE
