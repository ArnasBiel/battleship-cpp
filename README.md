# Battleship: C++ Console Game with Heatmap AI

A fully playable command-line implementation of the classic Battleship game, built in C++ as a university project. Features a human-vs-AI game mode where the AI opponent uses a **probability heatmap strategy** to make intelligent attack decisions.

---

## Gameplay

Players take turns placing ships on a 10×10 grid and then attacking each other's fleet. The game follows standard Battleship rules: a player continues shooting on a hit, and their turn ends on a miss or when they sink a ship.

```
| _ | A | B | C | D | E | F | G | H | I | J |
| 1 | _ | _ | _ | _ | _ | _ | _ | _ | _ | _ |
| 2 | _ | _ | _ | _ | _ | _ | _ | _ | _ | _ |
| 3 | _ | _ | _ | B | B | B | B | _ | _ | _ |
| 4 | _ | _ | _ | C | _ | _ | _ | _ | _ | _ |
...
```

---

## Project Structure

```
├── int_main_battleship.cpp   # Entry point and game loop
├── Board.h / Board.cpp       # Grid management, ship placement, attack tracking
├── Ship.h / Ship.cpp         # Individual ship state and hit detection
├── Player.h / Player.cpp     # Human player logic and input handling
├── AiPlayer.h / AiPlayer.cpp # AI opponent with heatmap strategy
└── Extras.h / Extras.cpp     # Coordinate conversion utility (e.g. "D5" → (3, 4))
```

---

## Class Overview

### `Board`
Manages two separate grids per player: a ship placement board and an attack tracking board. Handles validation of ship placements (boundary checks, overlap detection) and marks hits and misses after each attack. Ships are displayed using the first letter of their name as a symbol.

### `Ship`
Stores the coordinates of each individual ship fragment. Rather than inferring hit state from the board (which would require deducing ship size and orientation from surrounding marks), each ship tracks its own fragment coordinates directly, marking a hit replaces the fragment's coordinates with `(-1, -1)`. A ship is considered sunk when all fragments are marked.

### `Player`
Handles ship placement (with interactive coordinate input), attack execution, and tracking how many ships remain. The core `processAttack` method is shared with `AIPlayer` through inheritance, keeping attack logic consistent across both players and avoiding code duplication.

### `AIPlayer`
Extends `Player` with a heatmap-based attack strategy. Instead of random guessing, the AI overlays all valid ship placements onto a weight matrix before each turn and attacks the highest-weighted cell.

---

## AI Strategy and Probability Heatmap

The AI maintains a 10×10 integer matrix (the heatmap) representing the likelihood of a ship occupying each cell.

### How the heatmap is built

Before each AI turn, `heatInitialization()` iterates over every ship and every valid position it could still occupy. Each valid placement increases the weight of the cells it would cover:

- A placement with no confirmed hits nearby: `+1` per cell
- A placement overlapping one confirmed hit: `+3` per cell
- Two confirmed hits: `+5` per cell (and so on — each additional hit adds 2)

Missed cells are excluded from valid placements entirely. The result is that cells near the centre of the board naturally accumulate more weight at the start of the game (more ship orientations can reach them), producing a statistically sound opening strategy without any hardcoded rules.

### Updating after a shot

After each attack, `updateHeat()` adjusts the heatmap:

- **Miss:** the cell is marked as excluded (`-2`) so it is skipped in future placement calculations
- **Hit (ship still floating):** the four adjacent cells receive `+10`. If an adjacent hit is detected, the cell on the **opposite side** receives `+15` instead, reflecting the likely ship orientation. If a hit is found **two cells away**, the cell between them receives `+15`, bridging the gap.
- **Hit (ship sunk):** no directional bonuses are applied — the ship is gone

### Emergent diagonal search

An interesting property of this approach: when no hits have been registered, the heatmap naturally produces a **diagonal attack pattern**. This emerges from the weight accumulation geometry rather than being programmed explicitly - diagonal patterns maximise coverage of distinct ship positions on a 10×10 grid.

```
Example heatmap (early game, no hits yet):
30 45 57 63 64 66 63 57 45 30
45 60 72 78 75 80 78 72 60 45
57 72 84 90 79 90 90 84 72 57
63 78 90 96 75 92 96 90 78 63
...
```

---

## Building and Running

### Requirements
- C++17 or later
- Any standard C++ compiler (g++, clang++, MSVC)

### Compile
```bash
g++ -std=c++17 -o battleship int_main_battleship.cpp Board.cpp Ship.cpp Player.cpp AiPlayer.cpp Extras.cpp
```

### Run
```bash
./battleship
```

You will be prompted to place your ships, then the game begins. The AI opponent is named **A. Smith**.

---

## Fleet

| Ship | Size |
|---|---|
| Carrier | 5 |
| Battleship | 4 |
| Cruiser | 3 |
| Submarine | 3 |
| Destroyer | 2 |

---

## Academic Context

Developed as a university C++ programming course project. The focus was on object-oriented design, class responsibility separation, and implementing a non-trivial AI strategy without machine learning.

---

## Author

**Arnas Bielinis** — MSc Computer Science (Cybersecurity), Aarhus University  
[GitHub](https://github.com/ArnasBiel) · [LinkedIn](https://linkedin.com/in/)
