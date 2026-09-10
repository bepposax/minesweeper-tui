#!/bin/bash

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

update_alias() {
    if ! $(grep -q "alias mines" $ALIASFILE); then
        echo -n "Appending alias to $ALIASFILE... "
        echo $ALIAS >> $ALIASFILE &&
        echo -e "Done\n\nRestart the terminal or type 'source $ALIASFILE' for changes to take effect."
    else
        echo "Alias 'mines' already exists."
        read -rp "Update alias 'mines'? " choice
        if [[ $choice = y* || $choice = Y* ]]; then
            echo -n "Removing old alias... "
            grep -v "alias mines" $ALIASFILE > temp && mv temp $ALIASFILE && echo "Done"
            update_alias
        else
            echo -e "Discard\nDone."
        fi
    fi
}

read -rp "Create the alias 'mines' to run the game? [Y/n] " choice
if [[ $choice = y* || $choice = Y* ]]; then
    ALIASFILE=~/.bash_aliases
    ALIAS="alias mines='\$(find -L ~ -type f -name minesweeper -executable -print -quit)'"

    # creates ALIASFILE if it doesn't exist
    [ -f $ALIASFILE ] || {
        echo -n "Creating '$ALIASFILE'... "
        touch $ALIASFILE && echo "Done"
    }
    # creates the alias 'mines' if it doesn't exist
    update_alias
else
    echo "Abort."
fi

exit 0
