#!/bin/bash

create_sym_link () {
  ln -s $1 $2 || echo "Failed to create symlink $2 -> $1" 
}

input="simlinks.txt"
while IFS= read -r line
do
  IFS="\t" read -ra split_string <<< "$line"
  echo $line
  echo "ln -s ${split_string[-1]}  ${split_string[0]}"

  create_sym_link ${split_string[0]} ${split_string[-1]}
done < "$input"


