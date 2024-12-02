# Client executable

file(GLOB_RECURSE SOURCES "./*.cpp")

add_executable(r-type_client ${SOURCES})

# Link common library
target_link_libraries(r-type_client PRIVATE common)

# Link SFML (automatically found by connan)
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)

target_link_libraries(r-type_client PRIVATE sfml-system sfml-window sfml-graphics)

# Include directories for client
target_include_directories(r-type_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
