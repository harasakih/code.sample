-------------------------------------------------
	call-graph2
-------------------------------------------------
1. discribe
　家系図を表示する。
  = http表示t、pdf(graphviz)、console

2.History
  2017.06.30	call-graphからforkして、Ver2へ
  	= 格納ディレクトリの見直し
  	= optparseを使って、起動スクリプトをまとまる
	* 完了
  2017.06.30	Kakeizu.Nodeinfの持ち方を変更
	= graphvizで表示する名前は、Nodeinf.split(";")[gvkakeizu.FnameIdx]としている
	  Nodeinf[ key ] = [ 'val1;val2;val3' ]を
	  Nodeinf[ key ] = [ 'key;val1;val2;val3' ]に変更
			nodeinf_read:Kekeizu.py
			visited_inq:Kakeizu.py
	= Sison/Sosen_MakeTreeでstopの扱いを変更(is not None and len(xx) !=0 )
		sison_makedigraph		sosen_makedigraph
		sison_maketree		sosen_maketree
  2017.07.01    クラスモジュール定義ファイルだけで、実行可能とした。
    exKakeizuを廃止し、Kakeizuを直接起動できるように変更
  2017.07.04    PDF版でstopper定義時、stopperが表示されない不具合を解消

-------------------------------------------------
	Install
-------------------------------------------------
0. python-install 例
[01]pygraphviz
	python setup.py install --include-path=h:\Graphviz2.38\include --library-path=h:\Graphviz2.38\lib\release\lib

[02]graphviz
	dl graphviz0.6 from

1. Directory
	current
		_version.txt		: バージョン情報
		_readme.txt			: このファイル

		run_misiyou.py		: 未使用モジュールの検索
		run_unknown.py		: 不明モジュールの検索
		run_digraph.py		: ネットワーク図の作成

	    # python-module(classを定義)
		Kakeizu.py			: class Kakeizu		家系図の基本モジュール
		Gvkakeizu.py		: class	Gvkakeizu	graphvizを使って、graphを作成
		HttpKakeizu.py		: class MyHandler	家系図をhttpで表示

		# 以下4モジュールは不要
		Jobstepdsn.py		: class JobstepDsn	JobstepDsnの基本モジュール --　削除済,Kakeizu.pyに取込済
		Gvjobstepdsn.py		: class Gvkakeizu	graphviz版
		exKakeizu.py		: 家系図作成executer --- 削除済
		exHttpKakeizu.py	: http家系図executer -- 削除済

	dfiles			: 入力ファイル
	ex	edge.csv			: graphを作成するエッジ（from,to)(call-called)(parent,child)
		nodeinf				: ノード情報.ライブラリ台帳.
		nodeinf2			: ノード情報2.CURD情報

	otfiles			: 出力ファイル
		Mishiyou.txt		: edgeなし,nodeinfあり	未使用モジュール
		Unknown.txt			: edgeあり,nodeinfなし	不明モジュール


-------------------------------------------------
	How to Use
-------------------------------------------------
1. python environment



2. sample
(1) stop not specified
 	./Kakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC --search=sison  01-家康
 	./Kakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC --search=sosen  15-慶喜

(2) stop is specified
	./Kakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC  --node=nodeinf.csv --col=1,2 --titl="who rumor" --stop=08-吉宗 --search=sison 03-家光
	./Kakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC  --node=nodeinf.csv --col=1,2 --titl="who rumor" --stop=03-家光 --search=sosen 08-吉宗

(3）Gvkakeizu
    ./Gvkakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC --search=sison  01-家康
 	./Gvkakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC --search=sosen  15-慶喜

	./Gvkakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC  --node=nodeinf.csv --col=1,2 --titl="who rumor" --stop=08-吉宗 --search=sison 03-家光
	./Gvkakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC  --node=nodeinf.csv --col=1,2 --titl="who rumor" --stop=03-家光 --search=sosen 08-吉宗

(4) HttpKakeizu
	./HttpKakeizu.py -i ./tokugawa --edge=edge.csv --kbn=PC --node=nodeinf.csv --col=1,2 --titl="who rumor" ""
	 http://localhost:8000/sosen/08-吉宗
	 http://localhost:8000/sison/01-家康

-------------------------------------------------
	How to Use pyConsole
-------------------------------------------------
:::::::::::::::::::::::::::::::::::::::
::	コンソールの起動と既存操作
:::::::::::::::::::::::::::::::::::::::
$ python
Python x.x.x

:::: データの読込み ::::
	from kakeizu import Kakeizu
	kakeizu=Kakeizu()
	kakeizu.edge_read( "./dfiles/edge.csv", "PC" )
	kakeizu.nodeinf_read("./dfiles/nodeinf.csv", [5,6,8,9,10], "")
	kakeizu.nodeinf2_read( "./dfiles/nodeinf2.csv", [1,2,3], "--DB--")

:::: call-graphの作成
	kakeizu.sison_maketree( 0, "", "__me__")
	otfile=open( "./work/__outfile__", "w")
	print >> otfile, kakeizu.Sison_DispTree()
	otfile.close()

## PGM情報
	kakeizu.visited_inq()
## DB-TBL
	kakeizu.visited2_inq()

:::::::::::::::::::::::::::::::::::::::
::	デバック
:::::::::::::::::::::::::::::::::::::::
>> kakeizu.Me							起点
>>> len( kakeizu.Edges )				edgeの数=親子関係の数
>>> print kakeizu.Sison[ "__me__" ]		自分の子孫

										特定の親子関係を検索
>>> for edge in kakeizu.Edges:
...     (p,c) = edge
...     if p == "__me__":
...             print edge
...

>>> for node in kakeizu.Tree:
...     (level, parent, me, errflg) = node
...     print node
...

-------------------------------------------------
	Doxygen
-------------------------------------------------
