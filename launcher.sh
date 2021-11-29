#!/usr/bin/sh


var=$(echo $(zenity --entry --title="PIRDF" \
--text="Veuillez indiquer l'action a effectuer" \
launch configure install update createlogs clean))

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
