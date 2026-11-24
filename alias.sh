#!/bin/bash

# Set `mines` alias

update_alias() {
    if grep -qF "$ALIAS" "$ALIASFILE"; then
        echo ""
        grep -F --color=auto "$ALIAS" "$ALIASFILE"
        echo -e "\nAlias is already set."
        echo -e "Done"
    elif ! grep -q "^alias mines=" "$ALIASFILE"; then
        echo -n "Appending alias to $ALIASFILE... "
        printf '%s\n' "$ALIAS" >> "$ALIASFILE" &&
        echo "Done" &&
        grep -F --color=auto "$ALIAS" "$ALIASFILE"
        
        [ -f "$RCFILE" ] || {
            echo -ne "\nCreating '$RCFILE'... "
            touch "$RCFILE" && echo "Done"
        }
        if ! grep -qF -e ". $ALIASFILE" -e ". ~/${ALIASFILE##*/}" "$RCFILE"; then
            echo -ne "\nSourcing '${ALIASFILE}' in '$RCFILE'... "
            printf '\nif [ -f %s ]; then\n. %s\nfi\n\n' "$ALIASFILE" "$ALIASFILE" >> "$RCFILE" &&
            echo "Done"
        fi
        echo -e "Restart the terminal or type '. ${ALIASFILE}' for changes to take effect."
    else
        echo -e "Alias 'mines' already exists.\n"
        echo -ne "-    "
        grep --color=never "^alias mines=" "$ALIASFILE"
        printf '+    %s\n\n' "$ALIAS"
        read -rp "Confirm update? [Y/n] " choice
        if [[ $choice != [nN]* ]]; then
            echo -n "Removing old alias... "
            tmp="$(mktemp)" &&
            grep -v "^alias mines=" "$ALIASFILE" > "$tmp"; mv "$tmp" "$ALIASFILE" &&
            echo "Done"
            update_alias
        else
            echo -e "Discard\nDone."
        fi
    fi
}

read -rp "Create the alias 'mines' to run the game? [Y/n] " choice
if [[ $choice != [nN]* ]]; then
    case "$(basename "${SHELL}")" in
        ash|sh|dash) ALIASFILE="${HOME}"/.ashrc;       
                     RCFILE="${HOME}"/.profile ;;
        *)           ALIASFILE="${HOME}"/.bash_aliases;
                     RCFILE="${HOME}"/.bashrc ;;
    esac
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
