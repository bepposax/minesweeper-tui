# Campo Minato

Campo Minato is a simple command-line implementation of the Minesweeper game in C.

![screen](https://github.com/bepposax/minesweeper/assets/43136113/5f5cb23b-a49d-4945-ba75-61815ebaf80d)

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
- Uncovering a cell reveals the number of mines in the neighboring cells.
- You can place a flag on a cell if you suspect it contains a mine.
- To play a move, enter the coordinates of the cell in the format `[row][column]`.
- To flag a cell, enter the coordinates in the format `[row][column]`, followed by an uppercase letter.
- The game ends when you uncover all the safe cells or uncover a mine.

## Files

- `campo_minato.h`: Header file containing function declarations and data structures.
- `campo_minato.c`: Source code file containing the game logic.
- `main.c`: Source code file containing the main function.
- `Makefile`: Makefile to compile and run the program.

## Usage

- `make`: Compiles the source code files and generates the executable file.
- `make run`: Runs the program.
- `make clean`: Deletes the executable file.

## Dependencies

- The program depends on the ANSI-colors library, which is included in the `lib` folder.

Note: This program has been tested on a Unix-like system with GCC installed. Compatibility with other systems may vary.

Feel free to modify the code and customize the game according to your preferences.
