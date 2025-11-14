# Define App as an executable
add_executable(Pac-Man)

# Specify source files for the executable
target_sources(Pac-Man
    PRIVATE
        Source/MainMenuComponents/MainMenuLayer.cpp
        Source/MainMenuComponents/MainMenuLayer.h

        Source/GameComponents/GameLayer.cpp
        Source/GameComponents/GameLayer.h
        Source/GameComponents/Tile.cpp
        Source/GameComponents/Tile.h
        Source/GameComponents/Board.cpp
        Source/GameComponents/Board.h
        Source/GameComponents/PacMan.cpp
        Source/GameComponents/PacMan.h
        Source/GameComponents/Tile_json.hpp
        Source/GameComponents/TileType_json.hpp

        Source/GameOptionsComponents/GameOptionsLayer.cpp
        Source/GameOptionsComponents/GameOptionsLayer.h



        Source/Main.cpp
)

# Add include directory for App-specific headers
target_include_directories(Pac-Man
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Link Core library to App (so App can use Core functionality)
target_link_libraries(Pac-Man
    PRIVATE
        Core
)


