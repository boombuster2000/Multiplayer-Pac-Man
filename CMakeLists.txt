cmake_minimum_required(VERSION 3.20)

project(Multiplayer-Pacman LANGUAGES CXX)

# Enforce C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Persistent FetchContent cache directory ---
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/.cmake_cache")

# Add subprojects
add_subdirectory(engine)
add_subdirectory(game)

# Define the source and destination resource directories
set(RESOURCE_SRC_DIR ${CMAKE_SOURCE_DIR}/resources)
set(RESOURCE_DST_DIR $<TARGET_FILE_DIR:Pac-Man>/resources)
set(BOARDS_SRC_DIR ${CMAKE_SOURCE_DIR}/resources/boards)
set(BOARDS_DST_DIR $<TARGET_FILE_DIR:Pac-Man>/resources/boards)

# Recursively collect all resource files
file(GLOB_RECURSE RESOURCE_FILES "${RESOURCE_SRC_DIR}/*")
file(GLOB_RECURSE BOARD_FILES "${BOARDS_SRC_DIR}/*")

# Create a custom target for copying resources
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCE_SRC_DIR}
        ${RESOURCE_DST_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${BOARDS_SRC_DIR}
        ${BOARDS_DST_DIR}
    DEPENDS ${RESOURCE_FILES} ${BOARD_FILES} # re-run if any resource changes
    COMMENT "Copying resource and board files..."
)

# Ensure the resources are copied before building Pac-Man
add_dependencies(Pac-Man copy_resources)

