# Minesweeper-TUI

Minesweeper-TUI is a command-line implementation of the [Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game) "Minesweeper (video game) - Wikipedia") game in C.

<img src="https://github.com/user-attachments/assets/06e40d84-5147-4fb6-85cf-143da5470c36" alt="menu" height="333" width="auto">&emsp;
<img src="https://github.com/user-attachments/assets/c2f42ab5-c1b6-4b62-841c-4c2751e78a5c" alt="game" height="333" width="auto">

## Instructions

### Running
1. Clone the repository or [download the executable](https://github.com/bepposax/minesweeper-tui/raw/refs/heads/main/minesweeper).
    ```bash
    git clone https://github.com/bepposax/minesweeper-tui && cd minesweeper-tui
    ```
2. Run the executable
    ```bash
    ./minesweeper
    ```

### Compiling

1. Clone the repository
2. Make sure you have the necessary requirements installed on your system:
   - [Make](https://www.gnu.org/software/make/ "Make - GNU Project - Free Software Foundation") tool - `make`
   - [GCC](https://gcc.gnu.org/ "GCC, the GNU Compiler Collection - GNU Project") compiler - `gcc`
   - [NCURSES](https://invisible-island.net/ncurses/ "NCURSES - New Curses") library - `libncurses-dev`
  * Automate the installation with 
    ```bash
    ./install.sh
    ```
    (works with *apt* / *dnf* / *pacman* / *zypper* / *apk* / *xbps* package managers)
3. Usage (make commands):
   - `make`: Compiles the source code files and generates the executable file.
   - `make run`: Runs the program.
   - `make clean`: Deletes the executable file.
   - `make test`: Compiles and runs the program in testing mode.

### Optional

* Save the alias `mines` with
  ``` bash
  ./alias.sh
  ```
  and run it as a command from any folder

## Gameplay

![demo](https://github.com/user-attachments/assets/89b0d376-1262-429e-92b2-3e20afd2ac28)
- The board is a grid of cells, some containing mines.
- Numbers indicate how many mines are adjacent to that cell.
- Flag suspected mines to track them and avoid accidental clicks.
- Left-click on a numbered cell to quickly uncover surrounding cells if all adjacent mines are flagged.
- Uncover all empty cells to win. Hitting a mine ends the game.

## Features

* **Full Mouse Control**: left to play, right to flag, middle to mark
* **Four Difficulties**: Beginner, Intermediate, Expert, Custom

  <img src="https://github.com/user-attachments/assets/0540ec68-9993-46bf-b3bf-403b8d22a09c" alt="custom-game" height="333" width="auto" />

  A full screen Custom game *[height: 100%, width: 100%, mines: 10%]*
* **Safe First Click**: the first move can never be a mine
* **Chording**: left-click a number cell to clear around it
* **Move History**: replay a finished game step by step with arrow keys
* **Adaptive UI**: responsive to window resizes in real time

## Controls

<table>
  <tr>
    <th rowspan="2">🖱️ <kbd>Left</th>
      <td>uncover an undiscovered cell ( <b>■</b> )</td>
      <tr>
        <td>clear around a numbered cell ( <b>2</b> ) if all its adjacent mines are flagged</td>
      </tr>
  </tr>
  <tr>
    <th>🖱️ <kbd>Right</th>
    <td>flag ( <b>⚑</b> ) an undiscovered cell as a mine spot</td>
  </tr>
  <tr>
    <th>🖱️ <kbd>Middle</th>
    <td>mark ( <b>?</b> ) an undiscovered cell as a possible mine spot</td>
  </tr>
  <tr>
    <th><kbd>H</th>
    <td>look at the history of the last played game</td>
  </tr>
  <tr>
    <th><kbd>R</th>
    <td>restart the current game</td>
  </tr>
  <tr>
    <th><kbd>N</th>
    <td>start a new game</td>
  </tr>
  <tr>
    <th><kbd>Q</th>
    <td>quit the game / exit the program</td>
  </tr>
</table>

## Notes

- *This program has been tested on Windows Subsystem for Linux (WSL) with GCC installed. Compatibility with other systems may vary.*  
- *To ensure proper rendering of the game board, it's recommended to use a monospace font that supports the Unicode symbols used by the game.*

Feel free to modify the code and customize the game according to your preferences.

---

[![Creative Commons License](https://i.creativecommons.org/l/by-nc/4.0/88x31.png)](http://creativecommons.org/licenses/by-nc/4.0/)  
© 2023-2024 Ivano Izzo  
This work is licensed under a [Creative Commons Attribution-NonCommercial 4.0 International License](http://creativecommons.org/licenses/by-nc/4.0/).
