cmake_minimum_required(VERSION 3.11)
project(LOTP)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(COPY assets DESTINATION .)
file(COPY assets DESTINATION Debug)

add_executable(LOTP 
    src/main.cc
    src/system.cc
    src/gui.cc

    src/gfx/sprite.cc
    src/gfx/text.cc
    src/gfx/nineslice.cc

    src/screens/main_menu.cc
    src/screens/controls_screen.cc

    src/tracelog.c
)

add_subdirectory(libs)

target_include_directories(LOTP PRIVATE src/)

# # Dependencies
# set(RAYLIB_VERSION 4.2.0)
# find_package(raylib ${RAYLIB_VERSION} REQUIRED)
# if (NOT raylib_FOUND) # If there's none, fetch and build raylib
#   include(FetchContent)
#   FetchContent_Declare(
#     raylib
#     URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
#   )
#   FetchContent_GetProperties(raylib)
#   if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
#     set(FETCHCONTENT_QUIET NO)
#     FetchContent_Populate(raylib)
#     set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
#     add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
#   endif()
# endif()

#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)

message("PLATFORM: " ${PLATFORM})

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()