cmake_minimum_required(VERSION 3.12)
project(Nova LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Find Wayland package
find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client)

# Find X11 package
pkg_check_modules(X11 REQUIRED x11)

# If Wayland is found, define NOVA_WAYLAND_BACKEND
if(WAYLAND_FOUND)
    add_definitions(-DNOVA_WAYLAND_BACKEND)
    message(STATUS "Found Wayland, enabling NOVA_WAYLAND_BACKEND")
else()
    message(STATUS "Wayland not found, NOVA_WAYLAND_BACKEND will not be enabled")
endif()

# If X11 is found, define NOVA_X11_BACKEND
if(X11_FOUND)
    add_definitions(-DNOVA_X11_BACKEND)
    message(STATUS "Found X11, enabling NOVA_X11_BACKEND")
else()
    message(STATUS "X11 not found, NOVA_X11_BACKEND will not be enabled")
endif()

add_library(Nova STATIC src/Nova/Nova.cpp)
target_include_directories(Nova PUBLIC src)

# If Wayland is found, add the Wayland include directories to Nova's include paths
if(WAYLAND_FOUND)
    target_include_directories(Nova PUBLIC ${WAYLAND_INCLUDE_DIRS})
    target_link_libraries(Nova PUBLIC ${WAYLAND_LIBRARIES})
endif()

# If X11 is found, add the X11 include directories to Nova's include paths
if(X11_FOUND)
    target_include_directories(Nova PUBLIC ${X11_INCLUDE_DIRS})
    target_link_libraries(Nova PUBLIC ${X11_LIBRARIES})
endif()

# Link to Flux (no change needed for Flux here)
target_link_libraries(Nova PUBLIC Flux)

add_executable(NovaSample src/Sample/main.cpp)
target_link_libraries(NovaSample PUBLIC Nova Rune)

# Link to Wayland libraries if found
if(WAYLAND_FOUND)
    target_link_libraries(NovaSample PUBLIC ${WAYLAND_LIBRARIES})
endif()

# Link to X11 libraries if found
if(X11_FOUND)
    target_link_libraries(NovaSample PUBLIC ${X11_LIBRARIES})
endif()
