# Minesweeper-TUI

Minesweeper-TUI is a simple command-line implementation of the [Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game)) game in C.

![screen](https://github.com/bepposax/minesweeper-tui/assets/43136113/e280b728-08a4-4248-960d-c481c7d217f4)

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
- Use the mouse to left-click on a cell to uncover it.
- Use the mouse to right-click on a cell to flag or unflag it as a potential mine.
- Uncover all non-mine cells to win the game.
- Uncover a mine to lose the game.
- Press 'q' to quit the game at any time.

## Files

- `minesweeper.h`: Header file containing function declarations and data structures.
- `minesweeper.c`: Source code file containing the game logic.
- `main.c`: Source code file containing the main function.
- `Makefile`: Makefile to compile and run the program.

## Requirements

- ncurses library

## Usage

- `make`: Compiles the source code files and generates the executable file.
- `make run`: Runs the program.
- `make clean`: Deletes the executable file.

## Dependencies

- The program depends on the ANSI-colors library, which is included in the `include` folder.

Note: This program has been tested on a Unix-like system with GCC installed. Compatibility with other systems may vary.

Feel free to modify the code and customize the game according to your preferences.
