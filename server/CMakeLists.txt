# Server executable

file(GLOB_RECURSE SOURCES "./*.cpp")

add_executable(r-type_server ${SOURCES})

# Link common library
target_link_libraries(r-type_server PRIVATE common)

# Link SFML (automatically found by Vcpkg)
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)

target_link_libraries(r-type_server PRIVATE sfml-system sfml-window sfml-graphics sfml-network)

# Include directories for server
target_include_directories(r-type_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
