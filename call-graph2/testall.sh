#!/bin/sh
#
#

./Kakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC --search=sison  01-家康
./Kakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC --search=sosen  15-慶喜

./Kakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC  --node=nodeinf.csv --col=1,2 --titl="who rumor memo" --stop=08-吉宗 --search=sison 03-家光
./Kakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC  --node=nodeinf.csv --col=1,2 --titl="who rumor memo" --stop=03-家光 --search=sosen 08-吉宗

./Gvkakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC --search=sison  01-家康
./Gvkakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC --search=sosen  15-慶喜

./Gvkakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC  --node=nodeinf.csv --col=1,2 --titl="who rumor memo" --stop=08-吉宗 --search=sison 03-家光
./Gvkakeizu.py -i ./tokugawa -o ./otdir --edge=edge.csv --kbn=PC  --node=nodeinf.csv --col=1,2 --titl="who rumor memo" --stop=03-家光 --search=sosen 08-吉宗

./run_digraph.py PC ./tokugawa/edge.csv 
./run_misiyou.py PC ./tokugawa/edge.csv ./tokugawa/nodeinf.csv
./run_unknown.py PC ./tokugawa/edge.csv ./tokugawa/nodeinf.csv

