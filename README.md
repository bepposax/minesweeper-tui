# Minesweeper-TUI

Minesweeper-TUI is a command-line implementation of the [Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game) "Minesweeper (video game) - Wikipedia") game in C.

<img src="https://github.com/bepposax/minesweeper-tui/assets/43136113/f2c89bec-b6ea-4704-a516-69b404109b00" alt="menu" height="333" width="auto">
<img src="https://github.com/bepposax/minesweeper-tui/assets/43136113/56122690-8f87-41f2-9647-b4e65805ef2f" alt="board" height="333" width="auto">

## Instructions

### Running

1. Clone the repository or [download the executable](https://github.com/bepposax/minesweeper-tui/raw/refs/heads/main/minesweeper).
2. Run the executable (`./minesweeper`).

### Compiling

1. Make sure you have the necessary requirements installed on your system:
   - [Make](https://www.gnu.org/software/make/ "Make - GNU Project - Free Software Foundation") tool - `make`
   - [GCC](https://gcc.gnu.org/ "GCC, the GNU Compiler Collection - GNU Project") compiler - `gcc`
   - [NCURSES](https://invisible-island.net/ncurses/ "NCURSES - New Curses") library - `libncurses-dev`
   > Run [`install.sh`](install.sh) to automate the installation (**apt** only)
1. Clone the repository or download the source code files.
2. Navigate to the directory containing the source code files.
3. Usage (make commands):
   - `make`: Compiles the source code files and generates the executable file.
   - `make run`: Runs the program.
   - `make clean`: Deletes the executable file.
   - `make test`: Compiles the program in testing mode.

## Gameplay

![demo](https://github.com/user-attachments/assets/89b0d376-1262-429e-92b2-3e20afd2ac28)
- The board is a grid of cells, some containing mines.
- Numbers indicate how many mines are adjacent to that cell.
- Flag suspected mines to track them and avoid accidental clicks.
- Left-click on a numbered cell to quickly uncover surrounding cells if all adjacent mines are flagged.
- Uncover all empty cells to win. Hitting a mine ends the game.

### Controls

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
    <th><kbd>H
    <td>look at the history of the last played game
  <tr>
    <th><kbd>R
    <td>restart the current game
  <tr>
    <th><kbd>N
    <td>start a new game
  <tr>
    <th><kbd>Q
    <td>quit the game or exit the program
</table>

## Notes

- *This program has been tested on Windows Subsystem for Linux (WSL) with GCC installed. Compatibility with other systems may vary.*  
- *To ensure proper rendering of the game board, it's recommended to use a monospace font that supports the Unicode symbols used by the game.*

Feel free to modify the code and customize the game according to your preferences.

---

[![Creative Commons License](https://i.creativecommons.org/l/by-nc/4.0/88x31.png)](http://creativecommons.org/licenses/by-nc/4.0/)  
© 2023-2024 Ivano Izzo  
This work is licensed under a [Creative Commons Attribution-NonCommercial 4.0 International License](http://creativecommons.org/licenses/by-nc/4.0/).
