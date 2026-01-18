cmake_minimum_required(VERSION 3.10)

project(asteroids VERSION 1.0 LANGUAGES CXX)

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

add_executable(asteroids ${SRC_FILES})

# --- Wbudowanie assets w .exe (Windows-friendly) -----------------------------
option(EMBED_ASSETS "Embed assets into the executable" ON)

if(EMBED_ASSETS)
    set(EMBEDDED_ASSETS_DIR "${CMAKE_BINARY_DIR}/generated")
    file(MAKE_DIRECTORY "${EMBEDDED_ASSETS_DIR}")

    set(EMBEDDED_ASSETS_H "${EMBEDDED_ASSETS_DIR}/embedded_assets.h")
    set(EMBEDDED_ASSETS_CPP "${EMBEDDED_ASSETS_DIR}/embedded_assets.cpp")

    file(WRITE "${EMBEDDED_ASSETS_H}" "#pragma once\n#include <cstddef>\n\nstruct EmbeddedAsset {\n    const char* name;\n    const unsigned char* data;\n    unsigned int size;\n    const char* ext;\n};\n\nconst EmbeddedAsset* FindEmbeddedAsset(const char* name);\n")
    file(WRITE "${EMBEDDED_ASSETS_CPP}" "#include \"embedded_assets.h\"\n#include <cstring>\n\n")

    file(GLOB ASSET_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/assets/*")

    set(EMBED_ASSET_TABLE "")
    foreach(ASSET_FILE IN LISTS ASSET_FILES)
        get_filename_component(ASSET_NAME "${ASSET_FILE}" NAME)
        get_filename_component(ASSET_EXT "${ASSET_FILE}" EXT)
        string(SUBSTRING "${ASSET_EXT}" 1 -1 ASSET_EXT_NO_DOT)

        string(REGEX REPLACE "[^A-Za-z0-9_]" "_" ASSET_VAR "${ASSET_NAME}")
        set(ASSET_VAR "asset_${ASSET_VAR}")

        file(READ "${ASSET_FILE}" ASSET_HEX HEX)
        string(REGEX REPLACE "([0-9A-Fa-f]{2})" "0x\\1," ASSET_HEX "${ASSET_HEX}")

        file(APPEND "${EMBEDDED_ASSETS_CPP}" "static const unsigned char ${ASSET_VAR}[] = {${ASSET_HEX}};\n")

        file(SIZE "${ASSET_FILE}" ASSET_SIZE)
        string(APPEND EMBED_ASSET_TABLE "    {\"assets/${ASSET_NAME}\", ${ASSET_VAR}, ${ASSET_SIZE}, \"${ASSET_EXT_NO_DOT}\"},\n")
    endforeach()

    file(APPEND "${EMBEDDED_ASSETS_CPP}" "\nstatic const EmbeddedAsset kEmbeddedAssets[] = {\n${EMBED_ASSET_TABLE}};\n")
    file(APPEND "${EMBEDDED_ASSETS_CPP}" "static const unsigned int kEmbeddedAssetsCount = sizeof(kEmbeddedAssets) / sizeof(kEmbeddedAssets[0]);\n\n")
    file(APPEND "${EMBEDDED_ASSETS_CPP}" "const EmbeddedAsset* FindEmbeddedAsset(const char* name) {\n    for (unsigned int i = 0; i < kEmbeddedAssetsCount; ++i) {\n        if (std::strcmp(kEmbeddedAssets[i].name, name) == 0) {\n            return &kEmbeddedAssets[i];\n        }\n    }\n    return nullptr;\n}\n")

    target_sources(asteroids PRIVATE "${EMBEDDED_ASSETS_CPP}")
    target_include_directories(asteroids PRIVATE "${EMBEDDED_ASSETS_DIR}")
endif()

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
target_link_libraries(asteroids PRIVATE ${RAYLIB_TARGET})

# Drobne ustawienia dla wygody deweloperskiej
if(MSVC)
    # na Windows wy��cz niepotrzebne warnings lub ustaw tryb wyj�cia konsoli je�li potrzebne
    target_compile_options(asteroids PRIVATE /W4)
else()
    target_compile_options(asteroids PRIVATE -Wall -Wextra -Wpedantic)
endif()
