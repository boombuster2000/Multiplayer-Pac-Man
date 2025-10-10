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

# Define the source and destination resource directories
set(RESOURCE_SRC_DIR ${CMAKE_SOURCE_DIR}/resources)
set(RESOURCE_DST_DIR $<TARGET_FILE_DIR:Pac-Man>/resources)

# Recursively collect all resource files
file(GLOB_RECURSE RESOURCE_FILES "${RESOURCE_SRC_DIR}/*")

# Create a custom target for copying resources
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCE_SRC_DIR}
        ${RESOURCE_DST_DIR}
    DEPENDS ${RESOURCE_FILES}  # re-run if any resource changes
    COMMENT "Copying resource files..."
)

# Ensure the resources are copied before building Ball
add_dependencies(Pac-Man copy_resources)
