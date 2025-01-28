#!/bin/bash

dependencies=(
    "make"
    "gcc"
    "libncurses-dev"
)

GREEN="\033[0;32m"
RESET="\033[0m"

echo "Checking dependencies..."
for d in ${dependencies[@]}; do
    if dpkg -s $d &>/dev/null; then
        echo -e "$GREEN$d$RESET is already installed"
    else
        echo -e "Installing $GREEN$d$RESET..."
        sudo apt install $d && echo "Done"
    fi
done

read -rp "Do you wish to create the alias 'mines' to run the game? [Y/n] " choice
if [[ $choice = y* || $choice = Y* ]]; then {
    ALIASFILE=~/.bash_aliases
    ALIAS="alias mines='cd $(find ~ -type d -name minesweeper-tui) && make run && cd - &>/dev/null'"

    # creates ALIASFILE if it doesn't exist
    [ -f $ALIASFILE ] || {
    echo -n "Creating '$ALIASFILE'... "
    touch $ALIASFILE && echo " Done"
    }
    # creates the alias 'mines' if it doesn't exist
    if ! $(grep -q "alias mines" $ALIASFILE); then {
        echo -n "Appending alias to $ALIASFILE... "
        echo $ALIAS >> $ALIASFILE &&
        echo -e "Done\nRestart the terminal for changes to take effect."
    }
    else
        echo "Alias mines already exists."
    fi
} 
else
    echo "Abort."
fi

exit 0
