cmake_minimum_required(VERSION 3.21)

# standards & flags
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")

include(GNUInstallDirs)

# info
project(
    ModernStaggerLock
    VERSION 1.1.7
    LANGUAGES CXX
)

# out-of-source builds only
if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

# in-place configuration
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
    @ONLY
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY
)

# source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)
source_group(
    TREE
    ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
    ${SOURCES}
)

source_group(
    TREE ${CMAKE_CURRENT_BINARY_DIR}
    FILES ${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
)

# dependencies
add_subdirectory(extern/CommonLibSSE-NG CommonLibSSE)
add_subdirectory(extern/DKUtil DKUtil)
find_package(spdlog CONFIG REQUIRED)

# cmake target
# runtime
add_library(
    ${PROJECT_NAME}
    SHARED
    ${SOURCES}
    ${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    .clang-format
    vcpkg.json
)

# include dir
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# linkage
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    CommonLibSSE::CommonLibSSE
    DKUtil::DKUtil
    spdlog::spdlog
)

# compiler def
if(MSVC)
    add_compile_definitions(_MBCS)

    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE
        /MP
        /await
        /W0
        /WX
        /permissive-
        /Zc:alignedNew
        /Zc:auto
        /Zc:__cplusplus
        /Zc:externC
        /Zc:externConstexpr
        /Zc:forScope
        /Zc:hiddenFriend
        /Zc:implicitNoexcept
        /Zc:lambda
        /Zc:noexceptTypes
        /Zc:preprocessor
        /Zc:referenceBinding
        /Zc:rvalueCast
        /Zc:sizedDealloc
        /Zc:strictStrings
        /Zc:ternary
        /Zc:threadSafeInit
        /Zc:trigraphs
        /Zc:wchar_t
        /wd4200 # nonstandard extension used : zero-sized array in struct/union
    )
endif()

# PCH
target_precompile_headers(
    ${PROJECT_NAME}
    PRIVATE
    src/PCH.h
)

# ─────── MO2 auto-deploy ───────
if(DEFINED ENV{MO2})
    if(ENABLE_SKYRIM_AE)
        set(MO2_MOD_NAME "${PROJECT_NAME}_AE")
    else()
        set(MO2_MOD_NAME "${PROJECT_NAME}_SE")
    endif()

    set(MO2_MOD_DIR "$ENV{MO2}/mods/${MO2_MOD_NAME}")
    set(MO2_PLUGIN_DIR "${MO2_MOD_DIR}/SKSE/Plugins")

    # ---- DLL + PDB ----
    add_custom_command(
        TARGET "${PROJECT_NAME}" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${MO2_PLUGIN_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:${PROJECT_NAME}>"
        "${MO2_PLUGIN_DIR}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_PDB_FILE:${PROJECT_NAME}>"
        "${MO2_PLUGIN_DIR}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
        COMMENT "Deploying ${PROJECT_NAME} to MO2: ${MO2_MOD_DIR}"
    )

    # ---- ini ----
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ModernStaggerLock.ini")
        add_custom_command(
            TARGET "${PROJECT_NAME}" POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/ModernStaggerLock.ini"
            "${MO2_PLUGIN_DIR}/ModernStaggerLock.ini"
            COMMENT "Copying ModernStaggerLock.ini to MO2"
        )
    endif()

    # ---- config folder ----
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ModernStaggerLock")
        add_custom_command(
            TARGET "${PROJECT_NAME}" POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/ModernStaggerLock"
            "${MO2_PLUGIN_DIR}/ModernStaggerLock"
            COMMENT "Syncing ModernStaggerLock/ to MO2"
        )
    endif()

    message(STATUS "MO2 detected. Mod dir: ${MO2_MOD_DIR}")
else()
    message(STATUS "MO2 env var not found. Skipping auto-copy.")
endif()

# called via !Rebuild.ps1
if(DEFINED GROUP)
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        FOLDER
        ${GROUP}
    )
endif()
