#!/bin/bash

pipe=$1


while true
do
    if read line <$pipe; then
        echo $line
    fi
done

echo "Reader exiting"
