# 🎮 Quoridor on LandTiger

Welcome to **Quoridor**, a classic abstract strategy board game implemented on the **LandTiger Board** using **Keil µVision**.

This project is a digital port of the 1997 board game *Quoridor*, developed for an embedded system using C. The game logic, board rendering, and input handling are tailored for the LandTiger emulator or board.

---

## 📌 Game Overview

**Quoridor** is a two-player strategy game. The goal is simple: **reach the opposite side of the board before your opponent** — but there's a twist. You and your opponent can also place walls to block each other’s path. However, you **must always leave at least one path open** to the goal.

---

## 🧠 Game Rules

- **Board**: 7x7 squares (13x13 matrix internally).
- **Tokens**: Two players (White and Red), each starting from the center of their respective edges.
- **Objective**: Reach the opposite side first.
- **Turns**: Alternate between players. On each turn, a player can:
  1. **Move** one square in a cardinal direction (↑ ↓ ← →).
  2. **Place a wall** (if they have any left).
- **Walls**:
  - Each player has **8 walls**.
  - Walls block movement but cannot fully trap a player.
  - Walls can be rotated and confirmed via user input.
- **Jumping**: Players can jump over each other if aligned and unblocked.
- **Time Limit**: Each player has **20 seconds per move**. If time runs out, the turn is skipped.

---

## 🛠️ Technical Details

### 📁 Project Structure

- Implemented in **C** using **Keil µVision**.
- Compatible with **LandTiger emulator** or **actual hardware**.
- Project folder naming: `quoridor_emulator.zip` or `quoridor_board.zip`.

### 🧾 Key Variables

| Variable        | Description                                                                 |
|-----------------|-----------------------------------------------------------------------------|
| `int m[13][13]` | Game matrix (0: empty, 1: player, 3: wall)                                  |
| `uint32_t moves[200]` | Encoded moves using 32-bit integers                                    |
| `int turnPlayer` | Indicates active player (1 or 2)                                            |
| `int confirmWall` | Wall mode: 0 = not active, 1 = moving, 2 = confirmed                       |
| `int positionWall` | Wall orientation: 1 = vertical, 2 = horizontal                            |
| `int modePlayer` | Player mode: 1 = move, 2 = wall placement                                   |
| `int visited[13][13]` | Used for DFS path validation (ensuring fair play and path availability) |

### 💾 Move Encoding (32-bit Format)

| Field                  | Bits | Description                                    |
|------------------------|------|------------------------------------------------|
| PlayerID               | 8    | 0 for Player 1, 1 for Player 2                |
| PlayerMove/Wall        | 4    | 0 = Move, 1 = Wall                            |
| Vertical/Horizontal    | 4    | 0 = Vertical, 1 = Horizontal (if wall)        |
| Y Position             | 8    | Y coordinate on the board                     |
| X Position             | 8    | X coordinate on the board                     |

---

## 🕹️ User Controls

| Action                         | Input                    |
|--------------------------------|--------------------------|
| Start game                    | `INT0` button            |
| Move selection                | `Joystick (UP/DOWN/LEFT/RIGHT)` |
| Confirm move                  | `Joystick SELECT`        |
| Enter wall placement mode     | `KEY1`                   |
| Rotate wall                   | `KEY2`                   |
| Cancel wall placement         | `KEY1` (again)           |

---

## 🎨 Graphics

- The 7x7 board is drawn using the `DISPLAY` library.
- Player tokens:
  - Player 1 → ⚪ White
  - Player 2 → 🔴 Red
- Highlighted moves are shown for clarity.
- Walls are drawn between squares and may be rotated visually.
- Messages (e.g., "No walls available", timeout alerts) are displayed on screen.

---

## 🧪 Emulator / Board Config

Due to known compatibility issues with **Parallels**, manual modification of the `landtiger.ini` file is required to configure the emulator.

⚠️ *If using Parallels: avoid using the GUI to configure the emulator settings as it might crash.*

---

## 🧳 How to Run

1. Open the project in **Keil µVision**.
2. Load the appropriate `.uvproj` file.
3. If using the **emulator**, ensure your configuration is manually adjusted in `landtiger.ini`.
4. Flash or emulate the application.
5. Start the game with `INT0`.

---

## 🧼 Final Notes

- Make sure to compile all `.c` and `.h` files before executing.
- Keep track of player turns and remaining walls.
- Ensure proper memory usage on the LandTiger (avoid overflow on arrays and moves).

---

## 📞 Contacts

If you have any questions, issues, or feedback, feel free to reach out to us:

[![Email](https://img.icons8.com/color/25/000000/gmail.png) ponzuoligiacomo@gmail.com](mailto:ponzuoligiacomo@gmail.com)

[![LinkedIn](https://img.icons8.com/color/25/000000/linkedin.png) Giacomo Ponzuoli](https://www.linkedin.com/in/giacomo-ponzuoli/)

[![GitHub](https://img.icons8.com/color/25/000000/github.png) Open an issue](https://github.com/giacomoponzuoli3/quoridor/issues)
