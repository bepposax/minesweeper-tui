# Minesweeper-TUI

Minesweeper-TUI is a simple command-line implementation of the [Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game) "Minesweeper (video game) - Wikipedia") game in C.

![difficultites](https://github.com/bepposax/minesweeper-tui/assets/43136113/aa55979f-3347-495a-83d3-df46db765349) ![board](https://github.com/bepposax/minesweeper-tui/assets/43136113/0ae39140-ea32-4c9c-b6b6-62fcfc3fc200 "Game board")

## Instructions

1. Make sure you have GCC (GNU Compiler Collection) installed on your system.
2. Clone the repository or download the source code files.
3. Open a terminal and navigate to the directory containing the source code files.
4. Compile the program by running the command `make`.
5. Run the program using the command `make run`.

## Gameplay

- The game board is represented by a grid of cells.
- The goal of the game is to uncover all the cells that do not contain mines.
- Each cell may either be empty or contain a mine.
- Uncover all non-mine cells to win the game.
- Uncover a mine to lose the game.

## Controls
- 🖱️ **Left-click** 
  - Uncovers an undiscovered cell ( ■ ).
  - Uncovers the surroundings of a numbered cell ( <span style=color:green>**2**</span>  ) if all its adjacent mines are flagged.
- 🖱️ **Right-click** 
  - Flags ( <span style=color:firebrick>**⚑**</span> ) an undiscovered cell as a mine spot.
  - Marks ( <span style=color:orange>**?**</span> ) a flagged cell as a probable mine spot. (*work in progress*)
- Press **Q** to quit the game or exit the program.
- Press **N** to start a new game.
- Press **R** to restart the current game. (*work in progress*)

## Requirements

- [ncurses](https://en.wikipedia.org/wiki/Ncurses "ncurses - Wikipedia") library

## Usage

- `make`: Compiles the source code files and generates the executable file.
- `make run`: Runs the program.
- `make clean`: Deletes the executable file.

## Notes

- *This program has been tested on Windows Subsystem for Linux (WSL) with GCC installed. Compatibility with other systems may vary.*  
- *To ensure proper rendering of the game board, it's recommended to use a monospace font that supports the Unicode symbols used by the game.*

Feel free to modify the code and customize the game according to your preferences.

---

[![Creative Commons License](https://i.creativecommons.org/l/by-nc/4.0/88x31.png)](http://creativecommons.org/licenses/by-nc/4.0/)

This work is licensed under a [Creative Commons Attribution-NonCommercial 4.0 International License](http://creativecommons.org/licenses/by-nc/4.0/).
