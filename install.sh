#!/bin/bash

# Install building dependencies (apt)

dependencies=(make gcc libncurses-dev)
installed=true
updated=true

sudo -v || exit 1
echo -n "Getting packages information... "
if sudo apt-get update &>/dev/null; then
    echo "Done";
else
    echo "Failed";
fi
apt list "${dependencies[@]}"
echo ""

for d in "${dependencies[@]}"; do
    if ! dpkg -s "$d" &>/dev/null; then
        read -rp "Install $d? [Y/n] " choice
        if [[ $choice = [yY]* ]] && sudo apt-get install "$d"; then
            echo "Done"
        else
            installed=false
        fi
    elif (($(apt list --upgradeable $d 2>/dev/null | wc -l) > 1)); then
        read -rp "Upgrade $d? [Y/n] " choice
        if [[ $choice = [yY]* ]] && sudo apt upgrade "$d"; then
            echo "Done"
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

"$(dirname "${BASH_SOURCE[0]}")/alias.sh"

exit 0