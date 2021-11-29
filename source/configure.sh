#!/usr/bin/sh

mkdir -p logs

gcc source/main.c -g -I /usr/include -lglut -lm -lGL -lpthread -o rdf
