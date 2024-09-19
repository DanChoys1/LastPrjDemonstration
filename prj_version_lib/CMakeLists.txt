cmake_minimum_required(VERSION 3.5)

project(prjversionlib VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
add_library(prjversionlib SHARED
    prj_version.h
    prj_version.cpp
    prj_version_dialog.h
    prj_version_dialog.cpp
    prj_vers_lib_res.qrc)

set(VERS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vers.txt)
add_custom_target(
    versTarget
    COMMAND git log -1 > ${VERS_FILE} || echo "no git repo"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

add_dependencies(prjversionlib versTarget)

target_link_libraries(prjversionlib PRIVATE Qt5::Core Qt5::Widgets Qt5::Gui)
