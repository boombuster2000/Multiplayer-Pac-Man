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

# --- Define Core Library ---
add_library(Core STATIC)

target_sources(Core
    PRIVATE
        Source/Components/Menu.cpp
        Source/Components/MenuOption.cpp
        Source/Components/RenderableObject.cpp
        Source/Components/Text.cpp

        Source/Core/Application.cpp
    PUBLIC
        Source/Core/Application.h
        Source/Core/Layer.h
        Source/Core/Event.h

        Source/Components/Enums.h
        Source/Components/Menu.h
        Source/Components/MenuOption.h
        Source/Components/RenderableObject.h
        Source/Components/Text.h
)

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Link Raylib into Core
target_link_libraries(Core
    PUBLIC
        raylib
)
