#!/bin/bash

dependencies=(
    "make"
    "gcc"
    "libncurses-dev"
)

echo -e "Checking dependencies...\n"
for d in ${dependencies[@]}; do
    if dpkg -s $d &>/dev/null; then
        echo "$d is already installed"
    else
        echo "Installing $d..."
        sudo apt install $d && echo "Done"
    fi
done

echo "---"

read -rp "Do you wish to create the alias 'mines'? [Y/n] " choice
if [[ $choice = y* || $choice = Y* ]]; then {
    ALIASFILE=~/.bash_aliases
    ALIAS="alias mines='cd $(find ~ -type d -name minesweeper-tui) && make run && cd -'"

    # creates ALIASFILE if it doesn't exist
    [ -f $ALIASFILE ] || {
    echo -n "Creating '$ALIASFILE'..."
    touch $ALIASFILE && echo " Done"
    }
    # creates the alias 'mines' if it doesn't exist
    if ! $(grep -q "alias mines" $ALIASFILE); then {
        echo "Appending \"$ALIAS\" to \"$ALIASFILE\""
        echo "alias mines='cd $(find ~ -type d -name minesweeper-tui) && make run && cd -'" >> ~/.bash_aliases && echo -e "Done\nRestart the terminal for changes to take effect."
    }
    else
        echo "The alias 'mines' is already defined"
    fi
} 
else
    echo "Abort."
fi

exit 0
