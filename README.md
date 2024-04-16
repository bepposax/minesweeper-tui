# Minesweeper-TUI

Minesweeper-TUI is a command-line implementation of the [Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game) "Minesweeper (video game) - Wikipedia") game in C.

<img src="https://github.com/bepposax/minesweeper-tui/assets/43136113/080719ff-c474-49e9-93ce-8e9c789acf3a" alt="menu" height="333" width="auto">
<img src="https://github.com/bepposax/minesweeper-tui/assets/43136113/56122690-8f87-41f2-9647-b4e65805ef2f" alt="board" height="333" width="auto">

## Instructions

1. Make sure you have the necessary [requirements](#requirements) installed on your system.
2. Clone the repository or download the source code files.
3. Open a terminal and navigate to the directory containing the source code files.
4. Compile the program by running the command `make`.
5. Run the program using the command `make run`.

## Gameplay

- The game board is represented by a grid of cells.
- Each cell may either be empty or contain a mine.
- The goal of the game is to uncover all the empty cells.
- Uncover a mine to lose the game.

## Controls

<table>
  <tr>
    <th>🖱️ <kbd>Left
    <td>uncover an undiscovered cell ( <b>■</b> )
  <tr>
    <td>
    <td>uncover the surroundings of a numbered cell ( <b>2</b> ) if all its adjacent mines are flagged
  <tr>
    <th>🖱️ <kbd>Right
    <td>flag ( <b>⚑</b> ) an undiscovered cell as a mine spot
  <tr>
    <th>🖱️ <kbd>Middle
    <td>mark ( <b>?</b> ) an undiscovered cell as a possible mine spot
  <tr>
    <th><kbd>Q
    <td>quit the game or exit the program
  <tr>
    <th><kbd>N
    <td>start a new game
  <tr>
    <th><kbd>R
    <td>restart the current game
</table>

## Requirements

- [Make](https://www.gnu.org/software/make/ "Make - GNU Project - Free Software Foundation") tool
- [GCC](https://gcc.gnu.org/ "GCC, the GNU Compiler Collection - GNU Project") compiler
- [NCURSES](https://invisible-island.net/ncurses/ "NCURSES - New Curses") library

## Usage

- `make`: Compiles the source code files and generates the executable file.
- `make run`: Runs the program.
- `make clean`: Deletes the executable file.
- `make test`: Compiles the program in testing mode.

## Notes

- *This program has been tested on Windows Subsystem for Linux (WSL) with GCC installed. Compatibility with other systems may vary.*  
- *To ensure proper rendering of the game board, it's recommended to use a monospace font that supports the Unicode symbols used by the game.*

Feel free to modify the code and customize the game according to your preferences.

---

[![Creative Commons License](https://i.creativecommons.org/l/by-nc/4.0/88x31.png)](http://creativecommons.org/licenses/by-nc/4.0/)  
© 2023-2024 Ivano Izzo  
This work is licensed under a [Creative Commons Attribution-NonCommercial 4.0 International License](http://creativecommons.org/licenses/by-nc/4.0/).
