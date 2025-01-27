#!/bin/bash

dependencies=(
    "gcc"
    "make"
    "libncurses-dev"
)

echo "Checking dependencies..."
for d in ${dependencies[@]}; do
    if dpkg -s $d &>/dev/null; then
        echo "$d is already installed"
    else
        echo "Installing $d..."
        sudo apt install $d && echo "Done"
    fi
done

exit 0