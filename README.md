# C++ Project - Kurai

## Table of Contents

- [Description](#description)
- [Main Features](#main-features)
- [Screenshots](#screenshots)
- [Installation](#installation)
    - [Requirements](#requirements)
    - [Build Instructions](#build-instructions)
- [How to Play](#how-to-play)
    - [Controls](#controls)
    - [Gameplay](#gameplay)
- [Project Structure](#project-structure)
- [Technical Details](#technical-details)
    - [Dependencies](#dependencies)
    - [Architecture](#architecture)
- [Credits](#credits)
- [Authors](#authors)

## Description

**Kurai** is a 2D top-down video game inspired by classic "Zelda-like" titles. The player controls a character in a dark fantasy world, exploring a map, fighting enemies (goblins, bats), and beating bosses. The game is developed in **C++** using the **Qt** framework and leverages **Tiled** and **Aseprite** for map and sprite designs.

## Main Features

### Core Gameplay
- 2D map exploration with pixel art graphics.
- Character movement with animations.
- Enemy combat (goblins, bats).
- Power collection system.
- Final boss fight.
- Scoring system.
- Sound and music integration.

### Technical Features
- Fluid character and enemy animations using sprite sheets and gif
- Immersive sound effects and background music that adapts to gameplay
- Custom map loading system supporting Tiled map editor format
- Precise collision system for gameplay interactions
- Track player performance and achievements

### Visual & Audio
- Handcrafted sprites and tilesets created with Aseprite
- Immersive audio experience with environmental sounds and combat effects

## Screenshots

*[Add screenshots of your game here - gameplay, menus, boss fights, etc.]*

## Installation

### Requirements

- **Operating System**: Linux
- **Qt Framework**: Qt 6.x or higher
- **Compiler**: GCC 9+ or Clang 10+ with C++17 support
- **Build System**: CMake 3.16+ (optional) or qmake
- **Memory**: 4GB RAM minimum

### Build Instructions

#### Option 1: Using Qt Creator (Recommended)

1. **Clone the repository:**
   ```bash
   git clone https://github.com/nchevalier44/Projet_Cpp.git
   cd kurai
   ```



#### Option 2: Command Line Build

1. **Clone and navigate:**
   ```bash
   git clone https://github.com/your-username/kurai.git
   cd kurai
   ```

#### Troubleshooting

- **Qt not found**: Ensure Qt is properly installed and `qmake` is in your PATH
- **Missing libraries**: Install development packages for Qt modules
- **Build errors**: Verify you have a C++17 compatible compiler

## How to Play

### Controls

| Action        | Key        |
|---------------|------------|
| Move Up       | Z / ↑      |
| Move Down     | S / ↓      |
| Move Left     | Q / ←      |
| Move Right    | D / →      |
| Attack        | Left Click |
| Interact      | F          |
| Pause Menu    | Escape     |
| Slash attack* | W          |
| Shield*       | X          |
| Missile*      | A |
*Once unlocked in the game
### Gameplay

1. **Starting Out**: Begin your journey at the bottom of the map. Start by exploring to find out what lies ahead.
2. **Exploration**: Move through different areas using the arrow keys or ZQSD. Look for hidden paths and secret areas.
3. **Combat**: Approach enemies. Different enemies have unique behaviors:
    - **Goblins**: Throw projectiles.
    - **Bats**: Fly towards the player and attack on contact
    - **Bosses**: Randomly moves and attacks, requiring strategy to defeat.

4. **Power Collection**: Find and collect power-ups scattered throughout the world to increase your strength and abilities.

5. **Progression**: Defeat enemies and explore to increase your score and unlock new areas.

## Project Structure

```

Project_cpp/ 
├── assets
│   ├── fonts
│   │   └── Jersey10-Regular.ttf
│   ├── images
│   │   ├── bat
│   │   │   ├── Bat-Attack1.png
│   │   │   ├── Bat-Attack2.png
│   │   │   ├── Bat-Die.png
│   │   │   ├── Bat-Hurt.png
│   │   │   ├── Bat-IdleFly.png
│   │   │   ├── Bat-Run.png
│   │   │   ├── Bat-Sleep.png
│   │   │   └── Bat-WakeUp.png
│   │   ├── characters
│   │   │   ├── Back_idle.png
│   │   │   ├── Back_walk.png
│   │   │   ├── Dead_head.png
│   │   │   ├── die.png
│   │   │   ├── Fire_head-export.png
│   │   │   ├── Fire_head.gif
│   │   │   ├── Fire_head.png
│   │   │   ├── Fireless_head.gif
│   │   │   ├── Front_idle.png
│   │   │   ├── Front_walk.png
│   │   │   ├── Left_idle.png
│   │   │   ├── Left_walk.png
│   │   │   ├── Missile_spellFade.gif
│   │   │   ├── Missile_spell.gif
│   │   │   ├── Missile_spellGrow.gif
│   │   │   ├── MissileSpell_icone.png
│   │   │   ├── Right_idle.png
│   │   │   ├── Right_walk.png
│   │   │   ├── Shield.gif
│   │   │   ├── Shield_icone.png
│   │   │   ├── Slash1.gif
│   │   │   ├── Slash2.gif
│   │   │   ├── Slash3.gif
│   │   │   ├── SlashSpell_icone.png
│   │   │   ├── SpellBox.png
│   │   │   └── SpellBoxSelected.png
│   │   ├── crystalKnight
│   │   │   ├── boss_attackLeft.png
│   │   │   ├── boss_attackRight.png
│   │   │   ├── boss_die.png
│   │   │   ├── boss_idle.png
│   │   │   ├── boss_teleport.png
│   │   │   └── boss_thunder.png
│   │   ├── goblin
│   │   │   ├── enemy1_attack.png
│   │   │   ├── enemy1_idle.png
│   │   │   ├── enemy1_missile_blow.gif
│   │   │   ├── enemy1_missile.gif
│   │   │   └── enemy1_moving.png
│   │   └── menu
│   │       ├── background.png
│   │       ├── button_animation.gif
│   │       ├── button_background.png
│   │       ├── dropdown_arrow.png
│   │       ├── dropdown_item.png
│   │       ├── Main_bg.png
│   │       ├── settings_background.png
│   │       ├── slider_bar.png
│   │       └── slider_handle.png
│   ├── maps
│   │   ├── layers
│   │   │   ├── bgDonjon.png
│   │   │   ├── bigTreeBottom.png
│   │   │   ├── bigTree.png
│   │   │   ├── bigTreeTop.png
│   │   │   ├── chest.png
│   │   │   ├── donjon.png
│   │   │   ├── firstFloor.png
│   │   │   ├── floorDonjon.png
│   │   │   ├── grass.png
│   │   │   ├── path.png
│   │   │   ├── Plant.png
│   │   │   ├── shadow.png
│   │   │   ├── spell.png
│   │   │   ├── stoneTablet.png
│   │   │   ├── tree2.png
│   │   │   ├── treeBorder.png
│   │   │   ├── treePassage.png
│   │   │   ├── tree.png
│   │   │   └── wallsDonjon.png
│   │   ├── mapDonjon.json
│   │   └── map.json
│   ├── musics
│   │   ├── game_music2.mp3
│   │   ├── game_music.mp3
│   │   ├── menu_music.mp3
│   │   └── start_menu_theme.wav
│   └── sounds_effects
│       ├── batDie.wav
│       ├── bossDie.wav
│       ├── bossHit.wav
│       ├── bossLightning.wav
│       ├── bossTp.wav
│       ├── button.wav
│       ├── firstSlashAttack.wav
│       ├── hitBat.wav
│       ├── hitGoblin.wav
│       ├── hitPlayer.wav
│       ├── minionDie.wav
│       ├── minionMissile.wav
│       ├── playerDie.wav
│       ├── playerMissileBlow.wav
│       ├── playerMissile.wav
│       ├── playerPowerUp.wav
│       └── thirdSlashAttack.wav
├── CMakeLists.txt
├── README.md
└── src
    ├── constants.h
    ├── core
    │   ├── AudioManager.cpp
    │   ├── AudioManager.h
    │   ├── FontManager.cpp
    │   ├── FontManager.h
    │   ├── GameScene.cpp
    │   ├── GameScene.h
    │   ├── HUD.cpp
    │   ├── HUD.h
    │   ├── main_menu
    │   │   ├── MainMenuButton.cpp
    │   │   ├── MainMenuButton.h
    │   │   ├── ScoreboardWidget.cpp
    │   │   ├── ScoreboardWidget.h
    │   │   ├── SettingsWidget.cpp
    │   │   ├── SettingsWidget.h
    │   │   ├── StartMenuScene.cpp
    │   │   ├── StartMenuScene.h
    │   │   ├── VolumeSlider.cpp
    │   │   ├── VolumeSlider.h
    │   │   ├── WindowSizeComboBox.cpp
    │   │   └── WindowSizeComboBox.h
    │   ├── MainView.cpp
    │   ├── MainView.h
    │   ├── MainWindow.cpp
    │   ├── MainWindow.h
    │   ├── ScoreManager.cpp
    │   └── ScoreManager.h
    ├── entities
    │   ├── Entity.cpp
    │   ├── Entity.h
    │   ├── NPC
    │   │   ├── Bat.cpp
    │   │   ├── Bat.h
    │   │   ├── CrystalKnight.cpp
    │   │   ├── CrystalKnight.h
    │   │   ├── Goblin.cpp
    │   │   ├── Goblin.h
    │   │   ├── GoblinProjectile.cpp
    │   │   └── GoblinProjectile.h
    │   ├── player
    │   │   ├── Player.cpp
    │   │   ├── Player.h
    │   │   ├── PlayerProjectile.cpp
    │   │   ├── PlayerProjectile.h
    │   │   ├── PlayerShield.cpp
    │   │   ├── PlayerShield.h
    │   │   ├── PlayerSlash.cpp
    │   │   └── PlayerSlash.h
    │   ├── Projectile.cpp
    │   └── Projectile.h
    ├── main.cpp
```

## Technical Details

### Dependencies

#### Core Dependencies
- **QtCore**: Core Qt functionality and event system
- **QtGui**: Graphics and windowing system
- **QtWidgets**: User interface components
- **QtMultimedia**: Audio playback and sound management

#### Development Tools
- **Tiled Map Editor**: For creating and editing game maps
- **Aseprite**: For creating and editing pixel art sprites
- **Git**: Version control system

### Architecture

The game follows a component-based architecture with clear separation of concerns:

- **Game Loop**: Implemented using Qt's event system and timers
- **Entity System**: Base entity class with derived player and enemy classes
- **Resource Management**: Centralized loading and caching of sprites, sounds, and maps
- **State Management**: Game states (menu, playing, paused, game over)
- **Collision System**: Spatial partitioning for efficient collision detection
- **Audio System**: Multi-channel audio with background music and sound effects


## Credits

### Art & Design
- Pixel art sprites created with Aseprite by [Maxime You](https://github.com/Maximey13)
- Maps designed using Tiled Map Editor by [Maxime You](https://github.com/Maximey13)
- Some assets by Cainos [itch.io](https://cainos.itch.io/)
- Musics tracks from [M. J. Hood Music](https://www.youtube.com/@m.j.hoodmusic8789)
- Bat sprites from [MonoPixelArt](https://monopixelart.itch.io/)
- 
### Third-Party Resources
- Qt Framework for application development
- Tiled for map editing capabilities
- Aseprite for sprite creation

### Special Thanks
- Chacallade for all the tips : [Chalacade](https://www.youtube.com/@Challacade)

## Authors

- **Nathan Chevalier** - *Lead Developer* - [GitHub Profile](https://github.com/nchevalier44)
- **Maxime You** - *Graphics, Animations and developper* - [GitHub Profile](https://github.com/Maximey13)


*Project developed as part of ISEN Yncrea Ouest - Nantes*

