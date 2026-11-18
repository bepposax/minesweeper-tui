#!/bin/bash

# Install building dependencies

. /etc/os-release
case "${ID:-} ${ID_LIKE:-}" in
    *debian*|*ubuntu*) pm=apt                     ;;
    *rhel*|*fedora*)   pm=dnf                     ;;
    *arch*)            pm=pacman                  ;;
    *suse*)            pm=zypper                  ;;
    *alpine*)          pm=apk                     ;;
    *) echo "unsupported distro: $ID" >&2; exit 1 ;;
esac

pkg_install() {
    case "${pm}" in
        apt)    sudo apt-get install -y "$@"               ;;
        dnf)    sudo dnf install -y "$@"                   ;;
        pacman) sudo pacman -Syu --needed --noconfirm "$@" ;;
        zypper) sudo zypper -n in "$@"                     ;;
        apk)    sudo apk add "$@"                          ;;
    esac
}

pkg_update() {
    case "${pm}" in
        apt)    sudo apt-get update &>/dev/null    ;;
        dnf)    sudo dnf makecache &>/dev/null     ;;
        zypper) sudo zypper -n refresh &>/dev/null ;;
        apk)    sudo apk update &>/dev/null        ;;
    esac
}

pkg_upgrade() {
    case "${pm}" in
        apt)    sudo apt-get install -y --only-upgrade "$@" ;;
        dnf)    sudo dnf upgrade -y "$@"                    ;;
        zypper) sudo zypper -n update "$@"                  ;;
        apk)    sudo apk upgrade "$@"                       ;;
    esac
}

pkg_list() {
    case "${pm}" in
        apt)    apt list "$@" 2>/dev/null ;;
        dnf)    dnf list "$@"             ;;
        zypper) zypper search -xs "$@"    ;;
        apk)    apk list "$@"             ;;
    esac
}

pkg_upgradable() {
    case "${pm}" in
        apt)    apt list --upgradable 2>/dev/null | grep -q "^$1/"        ;;
        dnf)    dnf -q check-update "$1" &>/dev/null; [ $? -eq 100 ]      ;;
        zypper) zypper -n list-updates 2>/dev/null | grep -qE "\| $1 +\|" ;;
        apk)    apk list -u | grep -q "^$1-"                              ;;
    esac
}

pkg_installed() {
    case "${pm}" in
        apt)        dpkg -s "$1" &>/dev/null     ;;
        dnf|zypper) rpm -q "$1" &>/dev/null      ;;
        apk)        apk info -e "$1" &>/dev/null ;;
    esac
}

case "$pm" in
    apt)        libncurses=libncurses-dev ;;
    dnf|zypper) libncurses=ncurses-devel  ;;
    pacman)     libncurses=ncurses        ;;
    apk)        libncurses=ncurses-dev    ;;
esac

dependencies=(make gcc "$libncurses")
installed=true
updated=true

sudo -v || exit 1

# pacman
if [[ $pm == pacman ]]; then
    read -rp "Synchronize, upgrade the system and install dependencies? [Y/n] " choice
    if [[ -z $choice || $choice = [yY]* ]]; then
        if pkg_install "${dependencies[@]}"; then
            echo "Done"
        else
            echo "Failed to install dependencies."
            exit 1
        fi
    else
        installed=false
        updated=false
        echo "Skipped."
    fi
# others
else
    echo -n "Getting packages information... "
    if pkg_update; then
        echo "Done";
    else
        echo "Failed.";
        exit 1
    fi
    pkg_list "${dependencies[@]}"
    echo ""

    for d in "${dependencies[@]}"; do
        if ! pkg_installed "$d"; then
            read -rp "Install '$d'? [Y/n] " choice
            if [[ -z $choice || $choice = [yY]* ]] && pkg_install "$d"; then
                echo "Done"
            else
                installed=false
            fi
        elif pkg_upgradable "$d"; then
            read -rp "Upgrade '$d'? [Y/n] " choice
            if [[ -z $choice || $choice = [yY]* ]] && pkg_upgrade "$d"; then
                echo "Done"
            else
                updated=false
            fi
        fi
    done
fi

if $installed; then
    echo -n "All dependencies are installed"
    $updated && echo -n " and updated"
    echo -e ".\n"
else
    echo -n "Some dependencies "
    if [[ $pm == pacman ]]; then
        echo -n "may be"
    else 
        echo -n "are"
    fi
    echo " missing."
    exit 1
fi

bash "$(dirname "${BASH_SOURCE[0]}")/alias.sh"

exit 0

