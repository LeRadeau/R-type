# Fetch Catch2 (automatically handled by Conan)
find_package(Catch2 3 REQUIRED)

# Test executable

file(GLOB_RECURSE SOURCES "./*.cpp")

add_executable(tests ${SOURCES})

# Link common library and Catch2
target_link_libraries(tests PRIVATE common Catch2::Catch2WithMain)

# Enable Catch2 auto-discovery
include(Catch)
catch_discover_tests(tests)
