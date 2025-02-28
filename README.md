# PixelPlatformer

![PixelPlatformer Demo]((https://www.youtube.com/watch?v=-IV1tDEqrr4))  
*A 2D tile-based sandbox game framework built from scratch in SDL.*

## Overview

*PixelPlatformer* is a 2D sandbox game framework I developed to explore game development and C++ programming. Inspired by sandbox games like *Growtopia*, it features a tile-based world where players can move, jump, break and place blocks, and manage an inventory—all built from the ground up using SDL (Simple DirectMedia Layer). Initially started in SFML and later ported to SDL, this project evolved into a flexible, extensible base for 2D games, showcasing my passion for coding and game design.

The game starts with a simple world (100x54 tiles) consisting of dirt, rock, and bedrock layers, where a player navigates with smooth physics and a camera system. Designed with scalability in mind, it’s a reusable framework that can be adapted for multiplayer, new mechanics, or entirely different game genres. I’ve paused development to focus on other priorities but would be delighted to collaborate with others interested in scaling this project further—whether for multiplayer enhancements, additional features, or a full game.

### Features

- **Tile-Based World**: A 100x54 grid with dirt (top), rock (middle), and bedrock (bottom) layers, rendered efficiently with SDL.
- **Player Mechanics**: Movement (left/right), jumping with gravity-based physics, and collision detection with world bounds and blocks.
- **Block Interaction**: Break blocks with the mouse (slot 0) and place blocks from the inventory (slots 1–9), with a range-based system.
- **Inventory System**: A 1x3 slot inventory (expandable) for collecting and using blocks, with an SDL_ttf-based UI displaying items and quantities.
- **Camera System**: Smoothly follows the player with adjustable smoothing factors and a vertical dead zone, clamped to world bounds.
- **Extensible Block System**: Blocks are tied to IDs (0 = air, 1 = grass, 2 = dirt, 3 = bedrock), with textures loaded into a vector. Add a new texture to `World::texturePaths`, and it’s automatically rendered—no code changes needed beyond the asset push.
- **Modular Design**: Clean separation of concerns (`Game`, `Player`, `World`, `Inventory`, `InventoryUI`) for easy modification or expansion.

### Project Structure
PixelPlatformer/
├── assets/            # Fonts, textures (blocks, player), etc.
├── include/           # Header files
│   ├── Game.h
│   ├── Inventory.h
│   ├── InventoryUI.h
│   ├── Player.h
│   └── World.h
├── src/               # Source files
│   ├── Game.cpp
│   ├── Inventory.cpp
│   ├── InventoryUI.cpp
│   ├── main.cpp
│   ├── Player.cpp
│   └── World.cpp
└── CMakeLists.txt     # Build configuration


- **assets/**: Contains all game assets (e.g., `player.png`, block textures `0.png`–`3.png`, `ARIAL.TTF` font).
- **include/**: Headers defining the game’s core classes.
- **src/**: Implementation files with cleanly separated logic.
- **CMakeLists.txt**: Configures the build for macOS (arm64), linking SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer.

### How It Can Be Reused and Scaled

*PixelPlatformer* is a **game framework** designed for reusability and scalability:
- **Add New Blocks**: Push a new texture path to `World::texturePaths` and assign an ID. The rendering system auto-handles it, making block expansion seamless.
- **Extend Mechanics**: Modify `Player::update()` to add new actions (e.g., crafting, combat) or tweak physics constants (gravity, jump strength).
- **Scale the World**: Adjust `World::WIDTH` and `HEIGHT` or implement dynamic generation in `World::World()`.
- **Multiplayer Potential**: The structure supports networked block updates and player interactions—ready to scale into a multiplayer sandbox with additional development.
- **UI Customization**: `InventoryUI` can be expanded with new visuals or layouts by tweaking `drawSlot()`.
- **Genre Shift**: Reuse the rendering, physics, and input systems for platformers, puzzles, RPGs, or other 2D games.

This framework can evolve into a full multiplayer sandbox, a single-player adventure, or anything in between. I’d be excited to collaborate with others to scale it further—whether adding multiplayer features, new gameplay systems, or transforming it into a unique title.

### Demo

Check out the gameplay demo on YouTube: [(https://www.youtube.com/watch?v=-IV1tDEqrr4)].  
The video showcases movement, block breaking/placing, inventory usage, and the camera system in action.

### Building and Running

#### Prerequisites
- CMake 3.14+
- SDL2, SDL2_image, SDL2_ttf, SDL2_mixer (installed via Homebrew on macOS: `brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer`)
- C++17 compiler (e.g., Xcode on macOS)

#### Steps
1. Clone the repo:
   ```bash
   git clone https://github.com/yourusername/PixelPlatformer.git
   cd PixelPlatformer
2. Build with CMake:
  mkdir build && cd build
  cmake ..
  make
3. Run:
  ./PixelPlatformer

Controls:
A/D: Move left/right
Space: Jump
Left Click: Break (slot 0) or place blocks (slots 1–9)
0–9: Select inventory slot
P: Print inventory (debug)
W: Print world grid (debug)

Future Potential

While paused, I envisioned PixelPlatformer growing into HavenCraft—a multiplayer sandbox focused on house-building and trading. Its extensible design supports adding networked worlds, new block types, crafting systems, or even shifting to other genres like platformers or RPGs.

Collaboration

I’m open to partnering with developers interested in scaling this project. Whether it’s implementing multiplayer, enhancing mechanics, or building a full game, I’d be thrilled to work together—feel free to reach out via GitHub or LinkedIn!

Learning Outcomes

This project taught me:

C++ (OOP, memory management, STL).
SDL ecosystem (rendering, input, UI with SDL_ttf).
Game loop design and delta-time physics.
Code structuring for scalability and reuse.

Built by Elmar Ismayilov, a coder, student passionate about game development and collaborative projects.
