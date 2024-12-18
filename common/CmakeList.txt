# Common library for shared code

file(GLOB_RECURSE SOURCES "./*.cpp")

add_library(common STATIC ${SOURCES})

# Include directories for the library
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(common PRIVATE -Wall -Wextra)

# Link SFML (automatically found by connan)
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)

target_link_libraries(common PRIVATE sfml-system sfml-window sfml-graphics)