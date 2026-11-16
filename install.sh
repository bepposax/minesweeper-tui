#!/bin/bash

# Install building dependencies

#####################
### Needs testing ###
#####################

. /etc/os-release
case "${ID_LIKE:-$ID}" in
    *debian*|*ubuntu*) pm=apt                       ;;
    *rhel*|*fedora*)   pm=dnf                       ;;
    *arch*)            pm=pacman                    ;;
    *suse*)            pm=zypper                    ;;
    *) echo "unsupported distro: $ID" >&2; exit 1   ;;
esac

pkg_install() {
    case "${pm}" in
        apt)    sudo apt-get install -y "$@"                ;;
        dnf)    sudo dnf install -y "$@"                    ;;
        pacman) sudo pacman -S --needed --noconfirm "$@"    ;;
        zypper) sudo zypper --non-interactive install "$@"  ;;
    esac
}

pkg_update() {
    case "${pm}" in
        apt)    sudo apt-get update &>/dev/null                     ;;
        dnf)    sudo dnf makecache &>/dev/null                      ;;
        pacman) sudo pacman -Sy --noconfirm &>/dev/null             ;;
        zypper) sudo zypper --non-interactive refresh &>/dev/null   ;;
    esac
}

pkg_upgrade() {
    case "${pm}" in
        apt)    sudo apt-get install -y --only-upgrade "$@" ;;
        dnf)    sudo dnf upgrade -y "$@"                    ;;
        pacman) sudo pacman -Syu --noconfirm                ;;
        zypper) sudo zypper --non-interactive update "$@"   ;;
    esac
}

pkg_list() {
    case "${pm}" in
        apt)    apt list "$@" 2>/dev/null   ;;
        dnf)    dnf list "$@"               ;;
        pacman) pacman -Ss "$@"             ;;
        zypper) zypper search "$@"          ;;
    esac
}

pkg_upgradable() {
    case "${pm}" in
        apt)    apt list --upgradable 2>/dev/null | grep -q "^$1/"                       ;;
        dnf)    dnf -q check-update "$1" &>/dev/null; [ $? -eq 100 ]                     ;;
        pacman) pacman -Qu "$1" &>/dev/null                                              ;;
        zypper) zypper --non-interactive list-updates 2>/dev/null | grep -qE "\| $1 +\|" ;;
    esac
}

pkg_installed() {
    case "${pm}" in
        apt)        dpkg -s "$1" &>/dev/null    ;;
        dnf|zypper) rpm -q "$1" &>/dev/null     ;;
        pacman)     pacman -Qi "$1" &>/dev/null ;;
    esac
}

case "$pm" in
    apt)        libncurses=libncurses-dev ;;
    dnf|zypper) libncurses=ncurses-devel  ;;
    pacman)     libncurses=ncurses        ;;
esac

dependencies=(make gcc "$libncurses")
installed=true
updated=true

sudo -v || exit 1
echo -n "Getting packages information... "
if pkg_update; then
    echo "Done";
else
    echo "Failed";
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

if $installed; then
    echo -n "All dependencies are installed"
    $updated && echo -n " and updated"
    echo -e ".\n"
else
    echo "Some dependencies are missing."
    exit 1
fi

bash "$(dirname "${BASH_SOURCE[0]}")/alias.sh"

exit 0
