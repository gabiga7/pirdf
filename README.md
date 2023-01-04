#Created by gabiga7

**HOW TO USE :**
First time installation:
    launcher
        ->install

**Update:**
    launcher
        ->update

**Start :**
    launcher
        ->start

**Report a bug :**
    launcher
        ->createlogs
    send .tar file to gabelga91@gmail.com with comments
    





**Manual installation :**
./source/install.sh

**Automatic configuration and compilation command :**
./source/configure.sh

**Manual compilation command :**
gcc source/main.c -g -I /usr/include -lglut -lm -lGL -lpthread -o rdf


**Manual execution command :**
./rdf


Radar window may takes up to 10 seconds to appear depending your device


Known issues :
-Memory leak
-Segfault sometimes
