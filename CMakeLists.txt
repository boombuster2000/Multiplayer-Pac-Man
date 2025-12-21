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

# Define the source directories
set(RESOURCE_SRC_DIR ${CMAKE_SOURCE_DIR}/resources)
set(PROFILES_SRC_DIR ${CMAKE_SOURCE_DIR}/profiles)

# Create a custom target for copying resources
add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Pac-Man>/resources"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Pac-Man>/profiles"

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${RESOURCE_SRC_DIR}"
        "$<TARGET_FILE_DIR:Pac-Man>/resources"

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROFILES_SRC_DIR}"
        "$<TARGET_FILE_DIR:Pac-Man>/profiles"

        COMMENT "Copying resource and profile files..."
        VERBATIM
)

# Ensure the resources are copied AFTER building Pac-Man (so the directory exists)
add_dependencies(copy_resources Pac-Man)