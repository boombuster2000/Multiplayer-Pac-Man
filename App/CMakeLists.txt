# Define App as an executable
add_executable(Pac-Man)

# Automatically find all source files
file(GLOB_RECURSE APP_SOURCES "Source/*.cpp")
file(GLOB_RECURSE APP_HEADERS "Source/*.h" "Source/*.hpp")

# Specify source files for the executable
target_sources(Pac-Man
    PRIVATE
        ${APP_SOURCES}
        ${APP_HEADERS}
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


