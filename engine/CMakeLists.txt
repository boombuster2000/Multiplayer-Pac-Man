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

# --- Define engine Library ---
add_library(engine STATIC)

# Automatically find all source files
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "include/engine/*.h" "include/engine/*.hpp")

target_sources(engine
    PRIVATE
        ${ENGINE_SOURCES}
    PUBLIC
        ${ENGINE_HEADERS}
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link Raylib into engine
target_link_libraries(engine
    PUBLIC
        raylib
        nlohmann_json
)
