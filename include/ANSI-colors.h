#ifndef ANSI_COLORS_H
#define ANSI_COLORS_H

// Regular text
#define BLK "\033[0;30m"
#define RED "\033[0;31m"
#define GRN "\033[0;32m"
#define YEL "\033[0;33m"
#define BLU "\033[0;34m"
#define MAG "\033[0;35m"
#define CYN "\033[0;36m"
#define WHT "\033[0;37m"

// Regular bold text
#define B_BLK "\033[1;30m"
#define B_RED "\033[1;31m"
#define B_GRN "\033[1;32m"
#define B_YEL "\033[1;33m"
#define B_BLU "\033[1;34m"
#define B_MAG "\033[1;35m"
#define B_CYN "\033[1;36m"
#define B_WHT "\033[1;37m"

// Regular underline text
#define U_BLK "\033[4;30m"
#define U_RED "\033[4;31m"
#define U_GRN "\033[4;32m"
#define U_YEL "\033[4;33m"
#define U_BLU "\033[4;34m"
#define U_MAG "\033[4;35m"
#define U_CYN "\033[4;36m"
#define U_WHT "\033[4;37m"

// Regular background
#define BG_BLK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GRN "\033[42m"
#define BG_YEL "\033[43m"
#define BG_BLU "\033[44m"
#define BG_MAG "\033[45m"
#define BG_CYN "\033[46m"
#define BG_WHT "\033[47m"

// High intensity text
#define H_BLK "\033[0;90m"
#define H_RED "\033[0;91m"
#define H_GRN "\033[0;92m"
#define H_YEL "\033[0;93m"
#define H_BLU "\033[0;94m"
#define H_MAG "\033[0;95m"
#define H_CYN "\033[0;96m"
#define H_WHT "\033[0;97m"

// Bold high intensity text
#define B_H_BLK "\033[1;90m"
#define B_H_RED "\033[1;91m"
#define B_H_GRN "\033[1;92m"
#define B_H_YEL "\033[1;93m"
#define B_H_BLU "\033[1;94m"
#define B_H_MAG "\033[1;95m"
#define B_H_CYN "\033[1;96m"
#define B_H_WHT "\033[1;97m"

// High intensity background
#define BG_H_BLK "\033[0;100m"
#define BG_H_RED "\033[0;101m"
#define BG_H_GRN "\033[0;102m"
#define BG_H_YEL "\033[0;103m"
#define BG_H_BLU "\033[0;104m"
#define BG_H_MAG "\033[0;105m"
#define BG_H_CYN "\033[0;106m"
#define BG_H_WHT "\033[0;107m"

// Custom colors
#define RED_ "\033[38;2;200;32;32m"
#define BG_RED_ "\033[48;2;200;32;32m"

// Reset
#define RESET "\033[0m"

#endif
