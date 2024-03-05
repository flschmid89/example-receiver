#!/bin/bash

export_simlink()
{
    if [ "$1" = "$2" ]; then
    echo "Strings are equal."
else
    echo "$1  $2" >> simlinks.txt
fi
}

find_simlinks() 
{
    echo "'$1'"
    if [ -z "$1" ]; then
        echo "EMPTY"
    else
        echo "Simlink $1"
        for j in $(find -L /usr -samefile $1); do export_simlink $1  $j ;done

    fi

    

}


for i in $(ldd $1 | awk '{ print $3; }'); do find_simlinks $i; done

