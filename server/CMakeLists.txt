# Server executable
file(GLOB_RECURSE SOURCES "./*.cpp")
add_executable(r-type_server ${SOURCES})

target_compile_options(r-type_server PRIVATE -Wall -Wextra -Wpedantic -g3)
# Link common library
target_link_libraries(r-type_server PRIVATE common)

# Link SFML
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
target_link_libraries(r-type_server PRIVATE sfml-system sfml-window sfml-graphics sfml-network)

# Include directories
target_include_directories(r-type_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
