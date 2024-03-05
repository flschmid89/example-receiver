#!/bin/bash

export_dependency() 
{
    echo "'$1'"
    cp $i /test/export/
    echo "$1" >> depends.txt
    

}
mkdir /test
mkdir /test/export

for i in $(ldd $1 | awk '{ print $3; }'); do export_dependency $i; done

