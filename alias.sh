#!/bin/bash

# Set `mines` alias

update_alias() {
    if grep -q "$ALIAS" "$ALIASFILE"; then
        echo ""
        grep -F --color=auto "$ALIAS" "$ALIASFILE"
        echo -e "\nAlias is already set."
        echo -e "Done"
    elif ! grep -q "^alias mines=" "$ALIASFILE"; then
        echo -n "Appending alias to $ALIASFILE... "
        printf '%s\n' "$ALIAS" >> "$ALIASFILE" &&
        echo -e "Done\n" &&
        grep -F --color=auto "$ALIAS" "$ALIASFILE"
        
        # check if bashrc sources bash_aliases
        BASHRC=~/.bashrc
        [ -f ~/.bashrc ] || {
            echo -ne "\nCreating '$BASHRC'... "
            touch "$BASHRC" && echo "Done"
        }
        echo -ne "\nSourcing '~/.bash_aliases' in '$BASHRC'... "
        grep -q ". ~/.bash_aliases" $BASHRC || 
        printf '\nif [ -f ~/.bash_aliases ]; then\n. ~/.bash_aliases\nfi\n\n' >> ~/.bashrc &&
        echo "Done"
        echo -e "Restart the terminal or type '. ~/.bash_aliases' for changes to take effect.\n"
    else
        echo -e "Alias 'mines' already exists.\n"
        echo -ne "-    "
        grep --color=never "^alias mines=" "$ALIASFILE"
        printf '+    %s\n\n' "$ALIAS"
        read -rp "Confirm update? [Y/n] " choice
        if [[ $choice = [yY]* ]]; then
            echo -n "Removing old alias... "
            tmp="$(mktemp)" &&
            grep -v "^alias mines=" "$ALIASFILE" > "$tmp" &&
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
