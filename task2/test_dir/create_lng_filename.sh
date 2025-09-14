#!/bin/bash

# Max filename length per component
NAME_MAX=255

PATH_MAX=4096

# Directory name of 255 'a's
dirname=$(printf 'a%.0s' $(seq 1 $NAME_MAX))

# Base path
base="./maxpath"
mkdir -p "$base"
cd "$base" || exit

# Initialize path length
current_len=$(pwd | wc -c)  # current absolute path length

# Create nested directories
while [ $((current_len + NAME_MAX + 1)) -lt $PATH_MAX ]; do
    mkdir "$dirname"
    cd "$dirname" || exit
    # update current length
    current_len=$(pwd | wc -c)
done

# Create final file to fill remaining length
remaining=$((PATH_MAX - current_len - 1))
# last file name created by only b s
filename=$(printf 'b%.0s' $(seq 1 $remaining))
touch "$filename"

# Show info
echo "Created path with length $PATH_MAX bytes:"
pwd
echo "$filename"

