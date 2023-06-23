cmake_minimum_required(VERSION 3.1)
project(kdeplasma-addons)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options("$<$<CONFIG:DEBUG>:-Wall;-Wextra;-pedantic>")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include(MacroLibrary)
include(MacroOptionalDependPackage)

macro_optional_find_package(KDE4Workspace)
macro_log_feature(KDE4WORKSPACE_FOUND "kdebase workspace" "KDE base workspace libraries" "http://www.kde.org" FALSE "" "Needed for building several Plasma plugins")

macro_optional_find_package(KdepimLibs)
macro_log_feature(KDEPIMLIBS_FOUND "kdepimlibs" "KDE PIM libraries" "http://www.kde.org" FALSE "" "Needed for building several Plasma plugins")
find_package(Boost)

macro_optional_find_package(OpenGL)
macro_log_feature(OPENGL_FOUND "OpenGL" "API for developing portable, interactive 2D and 3D graphics applications" "http://mesa3d.sourceforge.net" FALSE "" "STRONGLY RECOMMENDED: The 3D hardware acceleration available through the OpenGL API is used in applications ranging from graphics and modellers to screensavers and video players.")

macro_optional_find_package(KDeclarative)
macro_log_feature(KDeclarative_FOUND "kdeclarative" "KDE Declarative (QML) support from kdelibs" "http://www.kde.org" FALSE "" "Required by qmlwallpapers")

macro_optional_find_package(QJSON)
macro_log_feature(QJSON_FOUND "QJSon" "Used for various Internet runners, for parsing API responses, and the microblog dataengine" "http://qjson.sourceforge.net/" FALSE "" "")

macro_optional_depend_package(Marble "Marble")
if (DEPEND_PACKAGE_Marble)
  macro_optional_find_package(Marble)
else (DEPEND_PACKAGE_Marble)
  set (MARBLE_FOUND FALSE)
endif (DEPEND_PACKAGE_Marble)
macro_log_feature(MARBLE_FOUND "Marble" "Desktop Globe" "http://marble.kde.org" FALSE "" "RECOMMENDED: Marble is used to provide a desktop globe wallpaper for Plasma.")

macro_optional_find_package(DBusMenuQt)
macro_log_feature(DBUSMENUQT_FOUND "DBusMenuQt" "DBusMenu Qt" "https://launchpad.net/libdbusmenu-qt/" FALSE "" "Needed for Unity support in IconTasks.")

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
include_directories (${KDE4_INCLUDES}
                     ${KDE4WORKSPACE_INCLUDE_DIR}
                     ${KDEPIMLIBS_INCLUDE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}/libs)

add_subdirectory(libs)
add_subdirectory(applets)
add_subdirectory(dataengines)
add_subdirectory(runners)

add_subdirectory(scriptengines)
add_subdirectory(wallpapers)
add_subdirectory(containments)
add_subdirectory(cmake)

macro_display_feature_log()
