#!/bin/bash

# Install building dependencies (apt)

dependencies=(
    "make"
    "gcc"
    "libncurses-dev"
)

installed=true
updated=true

sudo echo -n "Getting packages information... "
sudo apt-get update &>/dev/null && echo "Done"
apt list ${dependencies[@]}

echo ""
for d in ${dependencies[@]}; do
    if ! dpkg -s $d &>/dev/null; then
        read -rp "Install $d? [Y/n] " choice
        if [[ $choice = y* || $choice = Y* ]]; then
            sudo apt install $d && echo "Done"
        else 
            installed=false
        fi
    elif (($(apt list --upgradeable $d 2>/dev/null | wc -l) > 1)); then
        read -rp "Upgrade $d? [Y/n] " choice
        if [[ $choice = y* || $choice = Y* ]]; then
            sudo apt upgrade $d && echo "Done"
        else
            updated=false
        fi
    fi
done

if $installed; then 
    echo -n "All dependencies are installed"
    $updated && echo -n " and updated"
    echo -e ".\n"
fi

"$(dirname ${BASH_SOURCE[0]})"/alias.sh

exit 0
