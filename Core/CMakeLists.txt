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

target_sources(Core
    PRIVATE
        Source/UIComponents/Menu.cpp
        Source/UIComponents/MenuOption.cpp
        Source/UIComponents/RenderableObject.cpp
        Source/UIComponents/Text.cpp
        Source/UIComponents/GridTile.cpp

        Source/Core/Application.cpp
        Source/Core/TexturesManager.cpp

    PUBLIC
        Source/Core/Application.h
        Source/Core/Layer.h
        Source/Core/Event.h
        Source/Core/TexturesManager.h

        Source/UIComponents/Enums.h
        Source/UIComponents/Menu.h
        Source/UIComponents/MenuOption.h
        Source/UIComponents/RenderableObject.h
        Source/UIComponents/Text.h
        Source/UIComponents/Grid.h
        Source/UIComponents/GridTile.h

        Source/DataTypes/Vector2Ex.h

        Source/Serialization/Enums_json.hpp
        Source/Serialization/Grid_json.hpp
        Source/Serialization/GridTile_json.hpp
        Source/Serialization/JsonConverters.hpp 
        Source/Serialization/RenderableObject_json.hpp
        Source/Serialization/Vector2Ex_json.hpp
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
