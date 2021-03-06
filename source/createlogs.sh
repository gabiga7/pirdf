#!/usr/bin/sh

script -c "gcc source/main.c -g -I /usr/include -lglut -lm -lGL -lpthread -o rdf" compilation-log
cat compilation-log | perl -pe 's/\e([^\[\]]|\[.*?[a-zA-Z]|\].*?\a)//g' | col -b > logs/compilation-log-format
rm compilation-log


script -c "./rdf" execution-log
cat execution-log | perl -pe 's/\e([^\[\]]|\[.*?[a-zA-Z]|\].*?\a)//g' | col -b > logs/execution-log-format
rm execution-log


script -c "gcc source/main.c -g -fsanitize=address -static-libasan -I /usr/include -lglut -lm -lGL -lpthread -o rdf" compilation-asan-log
cat compilation-asan-log | perl -pe 's/\e([^\[\]]|\[.*?[a-zA-Z]|\].*?\a)//g' | col -b > logs/compilation-asan-log-format
rm compilation-asan-log


script -c "./rdf" execution-asan-log
cat execution-asan-log | perl -pe 's/\e([^\[\]]|\[.*?[a-zA-Z]|\].*?\a)//g' | col -b > logs/execution-asan-log-format
rm execution-asan-log

cat ~/.gitconfig > logs/user
cat /proc/version > logs/gcc-version
cat /etc/os-release > logs/os-version

tar -czvf envoyer.tar.gz logs/
