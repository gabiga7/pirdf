#!/usr/bin/sh

var=$(zenity --width=50 --height=300 --list "launch"  "install" "update" "auto-config" "createlogs" "clean" --column="command" --text="Select action" --title="RDF")


if [ $var = "launch" ]; then
    ./source/launch.sh
fi

if [ $var = "update" ]; then
    ./source/update.sh
    notify-send "Update done"
fi

if [ $var = "auto-config" ]; then
    ./source/configure.sh
    notify-send "Auto configuration done"
fi

if [ $var = "install" ]; then
    ./source/install.sh
    ./source/update.sh
    ./source/configure.sh
    notify-send "Installation done, use launch to start the program"
fi

if [ $var = "createlogs" ]; then
    ./source/createlogs.sh
    notify-send "Logs created, send envoyer.gz.tar to gabelga91@gmail.com"
fi

if [ $var = "clean" ]; then
    rm rdf -f
    rm logs -r -f
    rm result* -f
    rm envoyer* -f
    notify-send "Repository cleaned"
fi
