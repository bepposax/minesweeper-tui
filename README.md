# Minesweeper-TUI

Minesweeper-TUI is a simple command-line implementation of the [Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game) "Minesweeper (video game)") game in C.

![board](https://github.com/bepposax/minesweeper-tui/assets/43136113/481b8b94-cf62-4377-878b-87686caee30e)

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

## Requirements

- ncurses library

## Usage

- `make`: Compiles the source code files and generates the executable file.
- `make run`: Runs the program.
- `make clean`: Deletes the executable file.

Note: This program has been tested on a Unix-like system with GCC installed. Compatibility with other systems may vary.

Feel free to modify the code and customize the game according to your preferences.
