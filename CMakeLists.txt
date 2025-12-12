cmake_minimum_required(VERSION 3.10)

project(MyCppProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opcjonalnie: u�yj submodu�u raylib w katalogu external/raylib
option(RAYLIB_USE_SUBMODULE "Use external/raylib subdirectory if present" OFF)

# Zbierz �r�d�a
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
)

add_executable(MyCppProject ${SRC_FILES} )

# --- Znajdowanie raylib w r�nych �rodowiskach --------------------------------
set(RAYLIB_TARGET "")

# 1) Najpierw spr�buj znale�� pakiet CMake (np. z vcpkg lub instalacji z config)
find_package(raylib CONFIG QUIET)
if(raylib_FOUND)
    if(TARGET raylib)
        set(RAYLIB_TARGET raylib)
    elseif(TARGET raylib::raylib)
        set(RAYLIB_TARGET raylib::raylib)
    endif()
endif()

# 2) Je�li nie znaleziono, na systemach UNIX spr�buj przez pkg-config
if(NOT RAYLIB_TARGET)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(RAYLIB_PKG QUIET raylib)
        if(RAYLIB_PKG_FOUND)
            add_library(raylib::pkg INTERFACE IMPORTED)
            target_include_directories(raylib::pkg INTERFACE ${RAYLIB_PKG_INCLUDE_DIRS})
            target_link_libraries(raylib::pkg INTERFACE ${RAYLIB_PKG_LIBRARIES})
            target_compile_options(raylib::pkg INTERFACE ${RAYLIB_PKG_CFLAGS_OTHER})
            set(RAYLIB_TARGET raylib::pkg)
        endif()
    endif()
endif()

# 3) Je�eli masz raylib jako submodule w external/raylib i w��czy�e� opcj�, dodaj go
if(NOT RAYLIB_TARGET AND RAYLIB_USE_SUBMODULE)
    if(EXISTS "${CMAKE_SOURCE_DIR}/external/raylib/CMakeLists.txt")
        add_subdirectory(external/raylib)
        if(TARGET raylib)
            set(RAYLIB_TARGET raylib)
        elseif(TARGET raylib::raylib)
            set(RAYLIB_TARGET raylib::raylib)
        endif()
    endif()
endif()

# 4) Je�eli dalej nie znaleziono � komunikat z instrukcjami
if(NOT RAYLIB_TARGET)
    message(FATAL_ERROR
      "raylib not found.\n"
      "Na Windows: zainstaluj raylib przez vcpkg i ustaw __CMake toolchain__ na\n"
      " C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake lub uruchom vcpkg integrate.\n"
      "Na Linux: zainstaluj pakiet dev i pkg-config (np. 'sudo apt install libraylib-dev pkg-config').\n"
      "Alternatywnie dodaj raylib jako submodule w external/raylib i ustaw RAYLIB_USE_SUBMODULE=ON."
    )
endif()

# Linkuj docelowy target raylib do aplikacji
target_link_libraries(MyCppProject PRIVATE ${RAYLIB_TARGET})

# Drobne ustawienia dla wygody deweloperskiej
if(MSVC)
    # na Windows wy��cz niepotrzebne warnings lub ustaw tryb wyj�cia konsoli je�li potrzebne
    target_compile_options(MyCppProject PRIVATE /W4)
else()
    target_compile_options(MyCppProject PRIVATE -Wall -Wextra -Wpedantic)
endif()
