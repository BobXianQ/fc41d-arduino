#!/bin/bash

echo $1
echo $2

$1/beken_packager  $1/config.json

cp all_2M.1220.bin $2
