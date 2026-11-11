#!/bin/bash

# Set `mines` alias

update_alias() {
    if ! $(grep -q "alias mines" $ALIASFILE); then
        echo -n "Appending alias to $ALIASFILE... "
        echo $ALIAS >> $ALIASFILE &&
        echo -e "Done\n" &&
        grep "$ALIAS" "$ALIASFILE" &&
        echo -e "\nRestart the terminal or type 'source $ALIASFILE' for changes to take effect."
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
    DIR_PATH="$(dirname $(readlink -f ${BASH_SOURCE[0]}))"
    FILEPATH="$DIR_PATH/minesweeper"
    ALIAS="alias mines='$FILEPATH'"


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