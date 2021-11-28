#!/usr/bin/sh

rm -r logs
mkdir logs

gcc graph.c -g -I /usr/include -lglut -lm -lGL -lpthread -o rdf
