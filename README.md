# **Galactic Baseballer**

## **Table of Contents**

* [Description](#bookmark=id.jpgon4hjsbph)  
* [Features](#bookmark=id.opahn9tdl1xy)  
* [Technology Stack](#bookmark=id.fwfonrmx92xn)  
* [Project Architecture](#bookmark=id.cc3zjkzcjayz)  
* [Installation](#bookmark=id.tt11htg0zoaf)  
* [How to Play](#bookmark=id.klnfqlf0mwjg)  
* [Visual Elements](#bookmark=id.hxca4wxarq14)  
* [Troubleshooting](#bookmark=id.yx3ulyjm39k)  
* [Future Enhancements](#bookmark=id.dlbwoig7pu3o)  
* [License](#bookmark=id.lv0dift85wef)  
* [Acknowledgements](#bookmark=id.vpr22st798ga)

## **Description**

**Galactic Baseballer** is a dynamic 2D, two-player, turn-based artillery game developed in C++ with the Raylib library. Set in the vastness of space, the game challenges players to take on the roles of sentient planets, engaging in a strategic duel of celestial proportions.

The core objective is to skillfully aim and launch a projectile—a powerful black hole—at your opponent to deplete their health and claim victory. The game combines precision aiming, power control, and active defense through player-controlled orbiting spaceships, creating a fun and engaging local multiplayer experience.

This project was developed as a comprehensive exercise in game development, covering physics simulation, game state management, asset integration, and user input handling within a well-structured C++ environment.

## **Features**

* **Turn-Based Artillery Combat:** Classic turn-based gameplay where players take turns launching projectiles.  
* **Skill-Based Launching:** Control the launch **angle** with the mouse and the **force** by holding the mouse button, allowing for precise, strategic shots.  
* **Active Defense System:** Each player controls an orbiting spaceship paddle to block and reflect incoming shots.  
  * Player 1 (Red/Lava Planet) uses **WASD**.  
  * Player 2 (Blue/Ice Planet) uses **Arrow Keys**.  
* **Dynamic Obstacles:** Random celestial bodies spawn periodically, creating an ever-changing battlefield that requires players to adapt their strategies.  
* **Health System:** Both players have 5 HP. The first to reduce their opponent's HP to zero wins the game.  
* **Complete Game Loop:** Features a full gameplay cycle, from the main game to a "Game Over" screen with options to **Rematch** or **Exit**.  
* **Rich 2D Visuals:** Utilizes a variety of high-quality 2D sprites for players, projectiles, paddles, obstacles, and backgrounds to create an immersive space theme.

## **Technology Stack**

**Galactic Baseballer** is built with a focus on performance and simplicity, using native code and a lightweight game development library.

* **Language:**  
  * **C++ (C++17):** Chosen for its performance, control over system resources, and strong typing, making it an industry standard for game development.  
* **Core Library:**  
  * **Raylib (v4.x):** A simple and easy-to-use library to enjoy videogames programming. It provides all the necessary tools for windowing, input handling, audio, and 2D/3D rendering without the overhead of larger engines.  
* **Build System:**  
  * **g++ Compiler:** The standard GNU C++ compiler used to build the project from source.  
  * **Bash Script (build.bash):** A simple script is provided to automate the compilation process on Linux/macOS systems.

## **Project Architecture**

The project follows a clean, object-oriented design that separates core responsibilities into distinct classes, promoting modularity and maintainability.

* **main.cpp (Game Manager):** This is the entry point and central hub of the application. It is responsible for:  
  * Initializing the game window and loading all assets (textures).  
  * Managing the main game loop and game states (PLAYING, GAME\_OVER).  
  * Handling all user input for aiming, shooting, and paddle movement.  
  * Orchestrating player turns and rendering all visual elements to the screen.  
* **Entity Class (Entity.h, Entity.cpp):** A versatile class that represents all interactive game objects.  
  * It is used to create the **players** (throwers), the defensive **paddles**, and the dynamic **polygon obstacles**.  
  * Handles object-specific data like position, size, texture, and health.  
  * Contains collision detection logic, including a simple circular check for player hits and a more complex rotated AABB check for the paddles.  
* **Physics Class (Phys.h, Phys.cpp):** A dedicated class for simulating the projectile's physics.  
  * A single instance of this class represents the **ball** (black hole).  
  * Manages projectile motion, including velocity, gravity, and bouncing off screen boundaries.  
  * Its launch() method provides a clean interface to fire the projectile, and its reset() method is key to managing the turn-based flow.

The game operates by having main.cpp collect user input, which is then used to either launch the Physics object or update an Entity's paddle position. The main loop continuously updates the projectile's position and checks for collisions between the Physics object and the various Entity objects.

## **Installation**

Follow these steps to set up and run **Galactic Baseballer** on your local machine. These instructions are primarily for a Linux-based environment (like Ubuntu).

### **Prerequisites**

Ensure you have the following installed on your system:

* **A C++ Compiler:** g++ is recommended.  
  sudo apt update  
  sudo apt install build-essential

* **Git:** For cloning the repository.  
  sudo apt install git

* **Raylib:** The core graphics library.  
  sudo apt install libraylib-dev

### **1\. Clone the Repository**

First, clone the project repository to your local machine:

git clone https://github.com/Abraham-Na70/Your-Repo-Name.git  
cd Your-Repo-Name

*(Note: Replace Your-Repo-Name with the actual name of your repository).*

### **2\. Build the Game**

The repository includes a build.bash script to simplify compilation.

Make the script executable:

chmod \+x build.bash

Run the build script:

./build.bash

This will compile the source files (main.cpp, Entity.cpp, Phys.cpp) and link them against the Raylib library, producing an executable file named a.out.

### **3\. Run the Application**

Once the build is complete, you can run the game:

./a.out

## **How to Play**

**Galactic Baseballer** is a two-player local game.

* **Objective:** Be the last planet standing by hitting your opponent with the black hole until their HP runs out.  
* **Shooting (Active Player):**  
  * **Aim:** Move your mouse to aim the trajectory of your shot.  
  * **Charge Power:** Press and hold the **Left Mouse Button**. A gauge at the top of the screen will fill up.  
  * **Fire:** Release the **Left Mouse Button** to launch the black hole.  
* **Defense (Both Players):**  
  * **Player 1 (Left, Red):** Use the **W, A, S, D** keys to move your orbiting spaceship.  
    * A / D: Rotate the spaceship around your planet.  
    * W / S: Increase / Decrease the orbital distance.  
  * **Player 2 (Right, Blue):** Use the **Arrow Keys** to move your orbiting spaceship.  
    * Left / Right: Rotate the spaceship.  
    * Up / Down: Increase / Decrease the orbital distance.

A successful block will reflect the ball and immediately switch the turn\!

## **Visual Elements**

Below are placeholders for screenshots of the game in action.

*(Placeholder: A screenshot of the main gameplay, showing both players, the ball in mid-air, and the UI.)*

*(Placeholder: A screenshot of the Game Over screen, showing the winner and the rematch/exit buttons.)*

## **Troubleshooting**

| Issue | Description | Potential Solution(s) |
| :---- | :---- | :---- |
| **Compilation Fails / Build Error** | The build.bash script fails with linker errors. | 1\. Ensure Raylib is correctly installed. Run sudo apt install libraylib-dev. 2\. Verify that g++ is installed. |
| **Game Does Not Launch** | The executable is created but does not run. | Check console for errors. This could be due to missing runtime dependencies if Raylib was not installed system-wide. |
| **Textures Are Missing** | The game runs, but objects are colored shapes. | Ensure the src/img directory is in the same location relative to the executable, as the asset paths are hardcoded. |

## **Future Enhancements**

This project has a solid foundation with many opportunities for future enhancements:

* **Collision with Obstacles:** Implement collision detection between the projectile and the randomly spawned celestial bodies.  
* **Sound Effects & Music:** Add sound effects for launching, collisions, and paddle reflections, along with background music to enhance immersion.  
* **Main Menu:** Create a main menu screen before the game starts for a more polished user experience.  
* **Power-ups:** Introduce temporary power-ups, such as a larger paddle, a faster shot, or a multi-shot projectile.  
* **Single-Player vs. AI:** Develop an AI opponent for a single-player mode.  
* **More Diverse Levels:** Create different background scenes or levels with unique gravitational properties.

## **License**

This project is licensed under the MIT License. See the LICENSE file for details.

## **Acknowledgements**

* **Developer:** [Abraham-Na70](https://github.com/Abraham-Na70)  
* **Core Technology:** A huge thank you to [Ramon Santamaria](https://github.com/raysan5) for creating and maintaining the fantastic [Raylib library](https://www.raylib.com/).  
* **Assets:** Visual assets used in this project were sourced from various packs, including:  
  * *SBS \- 2D Planet Pack*  
  * *space\_background\_pack*  
  * *spaceships* pack  
  * *Buttons-PNG* pack