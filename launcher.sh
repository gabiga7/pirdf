#!/usr/bin/sh

cd ~/Documents/perso/h/compinion/radio/pirdf

var=$(zenity --width=50 --height=300 --list "launch" "configure" "install" "update" "createlogs" "clean" --column="command" --text="Select action" --title="RDF")


if [ $var = "launch" ]; then
    ./source/launch.sh
fi

if [ $var = "update" ]; then
    ./source/update.sh
fi

if [ $var = "configure" ]; then
    ./source/configure.sh
fi

if [ $var = "install" ]; then
    ./source/install.sh
fi

if [ $var = "createlogs" ]; then
    ./source/createlogs.sh
fi

if [ $var = "clean" ]; then
    rm rdf
    rm logs -r
fi
