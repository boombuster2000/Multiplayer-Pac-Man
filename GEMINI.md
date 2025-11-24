# Multiplayer Pac-man

This project is a multiplayer Pac-man game developed in C++. It uses the raylib library for graphics and input, and nlohmann/json for serialization.

## Project Overview

The project is structured into two main components: `engine` and `game`.

*   **engine**: This is the core of the application, providing a basic framework for creating a 2D game. It includes an `Application` class, a layer-based system for managing game states, and a simple UI system.
*   **game**: This contains the actual implementation of the Pac-man game. It defines the game logic, game components (like the board, Pac-man, and pellets), and the different game layers (menus, game, etc.).

The project uses a layer-based architecture, where each layer represents a different part of the game (e.g., main menu, game, overlay). The layers are pushed and popped from a stack to control the game flow.

The game board can be created programmatically or loaded from a JSON file, which allows for easy level design.

## Building and Running

The project uses CMake for building. To build and run the project, follow these steps:

1.  **Configure CMake**:
    ```bash
    cmake -S . -B build
    ```

2.  **Build the project**:
    ```bash
    cmake --build build
    ```

3.  **Run the game**:
    ```bash
    ./build/game/Pac-Man
    ```

## Development Conventions

*   **C++ Standard**: The project uses C++20.
*   **Coding Style**: The code follows the a consistent style, with a focus on readability and maintainability.
*   **Dependencies**: The project uses `raylib` for graphics and `nlohmann/json` for JSON serialization. These dependencies are fetched automatically using `FetchContent` in CMake.
