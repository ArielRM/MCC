#!/bin/bash

mainTXT="\
/*\n\
 * main.c\n\
 *\n\
 * Created: %s\n\
 * Author: %s\n\
*/\n\n\
"

if [ ! $# == 1 ]; then echo "Usage: $0 project_name"
elif [ -d $1 ]; then echo "Project $1 already exists"
else

    mkdir $1 $1/src $1/include $1/.vscode

    cp -R .default/c_cpp_properties.json $1/.vscode/c_cpp_properties.json
    cp -R .default/tasks.json $1/.vscode/tasks.json

    printf "NAME=%s\n" $1 > $1/Makefile
    cat .default/Makefile >> $1/Makefile

    printf "$mainTXT" "$(date)" "$USER" >> $1/src/main.c
    cat .default/main.c >> $1/src/main.c
    code $1 -g $1/src/main.c:13:5
    
fi
