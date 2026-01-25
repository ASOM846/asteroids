# Asteroids Game

A modern take on the classic Asteroids arcade game with RPG-style progression, multiple game modes, and engaging gameplay mechanics.

## Features

### Game Modes
- **Quick Start**: Practice mode with no pressure - perfect for testing upgrades and improving your skills
- **Arcade Mode**: Endless survival with progressive waves - compete for high scores!
- **Campaign Levels**: Story-driven missions with unique objectives:
  - Survive Asteroid Fields
  - Destroy Asteroids
  - Enemy Invasion
  - Ship Escort Missions
  - Boss Fights
  - Base Protection
  - Pursuit Missions

### Meta-Progression System
Build your ship over time with persistent upgrades:
- **Max Health** - Increase your ship's durability (10 levels)
- **Max Shield** - Boost shield capacity (10 levels)
- **Max Ammo** - Carry more ammunition (10 levels)
- **Fire Rate** - Shoot faster (5 levels, +15% per level)
- **Damage** - Deal more damage (5 levels, +20% per level)
- **Speed** - Move faster (5 levels, +10% per level)
- **Turn Speed** - Turn sharper (5 levels, +15% per level)
- **Shield Regen** - Regenerate shields over time (5 levels, 2 shield/sec per level)

Earn credits by destroying enemies and asteroids, then spend them in the Upgrade Shop!

### Combat System
- **Combo System**: Chain kills within 3 seconds to build massive combos
  - Bonus score increases with each consecutive kill
  - Score multiplier up to 2.15x at 10 combo (and growing!)
  - Visual feedback with color-coded combo counter
- **Dynamic Difficulty**: Arcade mode gets progressively harder with waves
- **Enemy Variety**: 3 enemy types (Basic, Fast, Tank) with unique stats
- **Power-Ups**: Health, Shield, and Ammo drops from destroyed objects

### Progression & Rewards
- **Currency System**: Earn credits from every kill
  - Asteroids: 1-3 credits based on size
  - Basic Enemies: 5 credits
  - Fast Enemies: 7 credits
  - Tank Enemies: 10 credits
- **High Score Tracking**: Compete against your best arcade runs
- **Level Unlocking**: Complete missions to unlock new challenges
- **Save System**: All progress, upgrades, and currency persist between sessions

## How to Play

### Controls
- **W** - Thrust forward
- **S** - Thrust backward
- **A/D** - Rotate ship
- **SPACE** - Fire weapons
- **ESC/P** - Pause game
- **F11** - Toggle fullscreen

### Tips for Success
1. **Chain Your Kills**: Keep the combo going for maximum score and currency
2. **Invest Wisely**: Focus on upgrades that match your playstyle
3. **Practice First**: Use Quick Start to test builds before tackling Arcade mode
4. **Shield Regen is OP**: Higher levels make you nearly invincible
5. **Fire Rate + Damage**: Deadly combination for wave clearing

## Building the Game

### Prerequisites
- CMake 3.10 or higher
- C++23 compatible compiler
- raylib library

### Linux
```bash
sudo apt install libraylib-dev pkg-config
mkdir build && cd build
cmake ..
make
./asteroids
```

### Windows (vcpkg)
```bash
vcpkg install raylib
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

## Game Design Philosophy

This game transforms the classic Asteroids into an engaging experience with:
- **Clear goals**: Multiple game modes with different objectives
- **Meaningful progression**: Every playthrough makes you stronger
- **Risk/Reward balance**: Combo system rewards aggressive play
- **Replayability**: Arcade mode with infinite scaling difficulty
- **Player agency**: Build your ship your way through upgrades

## Credits

Original code structure by ASOM846
Enhanced with meta-progression, arcade mode, and combat systems

