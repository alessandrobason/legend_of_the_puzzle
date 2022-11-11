cmake_minimum_required(VERSION 3.11)
project(LOTP)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(LOTP 
    src/camera.cpp
    src/Main.cpp
    src/Map_room.cpp
    src/Player.cpp
    src/Framework/Animation.cpp
    src/Framework/Collision.cpp
    # src/Framework/EventManager.cpp
    # src/Framework/Interactable.cpp
    # src/Framework/Projectiles.cpp
    src/Framework/Room.cpp
    src/Framework/RoomManager.cpp
    # src/Framework/TextBox.cpp
    src/Framework/TileMap.cpp
    # src/Framework/Weapon.cpp
    # src/Framework/Events/EVENTfadeinout.cpp
    # src/Framework/Events/EVENTgetpiece.cpp
    # src/Framework/Events/EVENTmenu.cpp
    # src/Framework/Events/EVENTmoveto.cpp
    # src/Framework/Events/EVENTsay.cpp
    # src/Framework/Events/EVENTwin.cpp
    # src/Framework/GUI/GUIbox.cpp
    # src/Framework/GUI/GUIbutton.cpp
    # src/Framework/GUI/GUIcheckbox.cpp
    # src/Framework/GUI/GUIelement.cpp
    # src/Framework/GUI/GUImanager.cpp
    # src/Framework/GUI/GUItiledelement.cpp
    # src/GameObjects/Enemy.cpp
    # src/GameObjects/ManObject.cpp
    # src/GameObjects/Octorock.cpp
    # src/Interactables/ManInteractable.cpp
    # src/Interactables/Statue.cpp
    src/JSONparser/JSONparser.cpp
    # src/Menus/ControlsMenu.cpp
    # src/Menus/DeathScreen.cpp
    # src/Menus/DebugMenu.cpp
    # src/Menus/Ending.cpp
    # src/Menus/LeaderboardMenu.cpp
    # src/Menus/MainMenu.cpp
    # src/Menus/MainScreen.cpp
    # src/Menus/Option.cpp
    # src/Menus/PauseMenu.cpp
    # src/Menus/SplashScreens.cpp
)

add_subdirectory(libs)

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