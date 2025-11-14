include(FetchContent)

# --- Fetch Raylib from GitHub ---
# This downloads and makes Raylib available inside the build.
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5  # Pick a release tag (e.g., 5.0, master)
)

# Actually fetch + make Raylib available
FetchContent_MakeAvailable(raylib)


FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0  # Pick a release tag (e.g., 5.0, master)
)

FetchContent_MakeAvailable(nlohmann_json)

# --- Define Core Library ---
add_library(Core STATIC)

# Automatically find all source files
file(GLOB_RECURSE CORE_SOURCES "Source/*.cpp")
file(GLOB_RECURSE CORE_HEADERS "Source/*.h" "Source/*.hpp")

target_sources(Core
    PRIVATE
        ${CORE_SOURCES}
    PUBLIC
        ${CORE_HEADERS}
)

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Link Raylib into Core
target_link_libraries(Core
    PUBLIC
        raylib
        nlohmann_json
)
