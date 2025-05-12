# Adventure Quest Game (OOP)

A C++ text-based board game where players navigate through a grid, collect items, overcome obstacles, and race to reach the goal.

## 🎮 Game Overview

Adventure Quest is a turn-based strategy game for two players. Players move across a board collecting coins, buying helpful items, and placing hurdles to slow down their opponent. The first player to reach the center goal with the most points wins!

## ✨ Features

- 🎲 Dynamic game board that increases in size as players progress through levels
- 💰 Economy system with gold and silver coins that can be collected and spent
- 🛡️ Various helper items (Sword, Shield, Water, Key) to overcome obstacles
- 🔥 Strategic hurdle placement (Fire, Snake, Ghost, Lion, Lock) to block opponents
- 💾 Save and load game functionality
- 🏆 Multiple levels with increasing difficulty

## 🚀 How to Play

### Game Objective
The objective is to reach the center goal of the board before your opponent while collecting as many points as possible.

### Player Movement
- Player 1 starts at the bottom-right corner and moves upward/leftward
- Player 2 starts at the top-left corner and moves downward/rightward
- Players take turns moving one cell at a time

### Items and Hurdles
- **Gold Coins**: Worth 10 points
- **Silver Coins**: Worth 5 points
- **Helper Items**:
  - **Sword**: Defeats Snakes and Lions (40 points)
  - **Shield**: Protects against Ghosts (30 points)
  - **Water**: Extinguishes Fire (50 points)
  - **Key**: Unlocks Locks (12 silver)
- **Hurdles**:
  - **Fire**: Blocks for 2 turns, -50 points (50 points to place)
  - **Snake**: Blocks for 3 turns, moves player back 3 cells, -30 points (30 points to place)
  - **Ghost**: Blocks for 1 turn, -20 points (20 points to place)
  - **Lion**: Blocks for 4 turns, -50 points (50 points + 1 gold to place)
  - **Lock**: Blocks indefinitely until unlocked (12 silver to place)

## 🛠️ Installation

### Prerequisites
- C++ compiler (GCC, Clang, MSVC, etc.)
- Standard C++ libraries
- 

## 🎯 Game Controls
The game is played through text-based commands:
1. Move
2. Place Hurdle
3. Buy Helper
4. Use Item

## 🧩 Game Structure
- **Board**: Manages the game grid and items
- **Player**: Handles player state, inventory, and actions
- **Items**: Base class for all collectible objects
- **Helpers**: Special items that assist players
- **Hurdles**: Obstacles that block or hinder player progress
- **AdventureQuest**: Main game class that controls game flow


## 🤝 Contributing
Contributions, issues, and feature requests are welcome!
