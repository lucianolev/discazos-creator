#!/bin/bash

# Reencode dir with MP3 files to 128kbps CBR
# Version: 0.1
# Author: Luciano Leveroni

IFS=$'\n'

die () {
    echo >&2 "$@"
    exit 1
}

#Check one argument provided
[ "$#" -eq 1 ] || die "Usage: $0 mp3dir"

#Check that the argument is a valid dir
DESTINATION="${1%?}" #Removes last slash
[ -d $DESTINATION ] || die "Path '$DESTIONATION' does not exists!"

#Check if sox exists
command -v sox >/dev/null 2>&1 || die "Sox not found! Please install sox with 'apt-get install sox'"

SAVE_TO=$DESTINATION/128kbps

mkdir $SAVE_TO
echo "Encoding files in '$DESTINATION':"

for f in $DESTINATION/*.mp3
do
  filename=$(basename "$f")
  echo "Encoding... $filename"
  sox "$f" -C 128 $SAVE_TO/$filename #Encode to 128kbps CBR
done

echo "Finished! Encoded files saved to '$SAVE_TO'"

exit 0
