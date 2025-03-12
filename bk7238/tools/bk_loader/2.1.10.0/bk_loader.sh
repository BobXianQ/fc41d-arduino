#!/bin/bash

if [ $# -ne 4 ]; then
    echo "Usage: $0 <path_to_exe> <port_with_prefix> <baud_rate> <input_file>"
    exit 1
fi

path_to_exe=$1
port_with_prefix=$2
baud_rate=$3
input_file=$4

# Extract the port number after 'M' in the second argument
port_number=$(echo $port_with_prefix | sed 's/.*ttyUSB\([0-9]\+\)/\1/')


# Call the executable with the extracted port number
"$path_to_exe/bk_loader" download -p "$port_number" -b "$baud_rate" -i "$input_file" --fast-link 1
