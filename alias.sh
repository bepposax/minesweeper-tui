#!/bin/bash

# Set `mines` alias

update_alias() {
    if ! grep -q "^alias mines=" "$ALIASFILE"; then
        echo -n "Appending alias to $ALIASFILE... "
        printf '%s\n' "$ALIAS" >> "$ALIASFILE" &&
        echo "Done" &&
        grep -F --color=auto "$ALIAS" "$ALIASFILE"
        echo -e "\nRestart the terminal or type 'source $ALIASFILE' for changes to take effect.\n"
    else
        echo "Alias 'mines' already exists."
        grep -F --color=auto "$ALIAS" "$ALIASFILE"
        printf '\nReplace it with\n%s? [Y/n] ' "$ALIAS"
        read -r choice
        if [[ $choice = [yY]* ]]; then
            echo -n "Removing old alias... "
            tmp="$(mktemp)" &&
            grep -v  "^alias mines=" "$ALIASFILE" > "$tmp" &&
            mv "$tmp" "$ALIASFILE" &&
            echo "Done"
            update_alias
        else
            echo -e "Discard\nDone."
        fi
    fi
}

read -rp "Create the alias 'mines' to run the game? [Y/n] " choice
if [[ $choice = [yY]* ]]; then
    ALIASFILE=~/.bash_aliases
    DIR_PATH="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"
    FILEPATH="$DIR_PATH/minesweeper"
    ALIAS="alias mines='$FILEPATH'"

    # creates ALIASFILE if it doesn't exist
    [ -f "$ALIASFILE" ] || {
        echo -n "Creating '$ALIASFILE'... "
        touch "$ALIASFILE" && echo "Done"
    }
    update_alias
else
    echo -e "Abort\nDone."
fi

exit 0