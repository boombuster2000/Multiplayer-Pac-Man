# Define App as an executable
add_executable(Pac-Man)

# Specify source files for the executable
target_sources(Pac-Man
    PRIVATE
        Source/MainMenuLayer.cpp
        Source/MainMenuLayer.h
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
