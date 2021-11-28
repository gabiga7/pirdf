#!/usr/bin/sh

mkdir -p logs

gcc graph.c -g -I /usr/include -lglut -lm -lGL -lpthread -o rdf
