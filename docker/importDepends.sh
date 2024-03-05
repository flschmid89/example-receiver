#!/bin/bash
check_and_create_folder () {
  # Extrahiere den Ordnerpfad aus dem Dateinamen
  folder_path=$(dirname "$1")
  
  # Prüfe, ob der Ordnerpfad existiert, und erstelle ihn gegebenenfalls
  if ! [  -d "$folder_path" ]; then
    echo "Der Ordnerpfad $folder_path existiert nicht. Er wird erstellt."
    mkdir -p "$folder_path"
  fi
}

check_file_existence () {
  if ! [ -f "$1" ]; then
    echo "Die Datei $1 existiert nicht."
    check_and_create_folder $1

    cp $2 $1
  fi
}

input="depends.txt"
while IFS= read -r line
do
    IFS="/" read -ra split_string <<< "$line"

    echo "${split_string[-1]}"
  check_file_existence $line ${split_string[-1]}
done < "$input"

