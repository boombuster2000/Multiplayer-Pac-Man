# Multiplayer Pac-Man

A C++ and Raylib implementation of the classic arcade game, Pac-Man. This project features a custom-built 2D game engine and a modular design that separates core engine logic from the game itself.

## Features

*   Classic Pac-Man gameplay.
*   Player profiles and persistent highscores for each level.
*   Customizable levels via JSON board files.
*   A reusable 2D game engine built on top of Raylib.
*   Support for both keyboard and gamepad controls.

## Technologies Used

*   **Language:** C++20
*   **Build System:** CMake
*   **Graphics & Audio:** [Raylib 5.5](https://www.raylib.com/)
*   **JSON Parsing:** [nlohmann/json](https://github.com/nlohmann/json)

## Getting Started

### Prerequisites

*   A C++20 compatible compiler (e.g., GCC, Clang, MSVC).
*   CMake (version 3.20 or newer).

### Building and Running

The project uses CMake's `FetchContent` to download all dependencies automatically.

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    cd Multiplayer-Pac-Man
    ```

2.  **Configure the project with CMake:**
    ```bash
    cmake -S . -B build
    ```

3.  **Build the executable:**
    ```bash
    cmake --build build
    ```

4.  **Run the game:**
    The executable will be located in the build output directory.
    *   On Linux/macOS:
        ```bash
        ./build/game/Pac-Man
        ```
    *   On Windows:
        ```powershell
        .\build\game\Debug\Pac-Man.exe
        ```

## How to Play

*   **Movement:**
    *   **Keyboard:** `W`/`A`/`S`/`D` or Arrow Keys
    *   **Gamepad:** D-Pad or Left Analog Stick
*   **Confirm/Select:**
    *   **Keyboard:** `Enter`
    *   **Gamepad:** A / Cross Button
*   **Pause/Back:**
    *   **Keyboard:** `Escape`
    *   **Gamepad:** Start / Menu Button

## Project Structure

The codebase is organized into two main parts:

*   `/engine`: A reusable, game-agnostic static library that provides core functionalities like the main application loop, input handling, texture management, and a layer-based state system.
*   `/game`: The main Pac-Man executable. It contains all game-specific logic, components (Pac-Man, Ghosts, Board), and UI layers (Menus, HUD).

Board layouts are stored as JSON files in `/resources/boards/`, and player data is saved in `/profiles/`.
