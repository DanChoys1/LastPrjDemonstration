cmake_minimum_required(VERSION 3.5)
project(RLS-MR231 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-include QtDebug)
endif()

include_directories(window/)
include_directories(window/menu/)
include_directories(display/)
include_directories(display/noize_emulator/)
include_directories(coordinates/)
include_directories(input_controls/)
include_directories(input_controls/devices/)
include_directories(data_base/)
include_directories(data/)
include_directories(common/)
include_directories(math/)

FILE(GLOB PROJECT_SOURCES   *.ui *.h *.cpp
			    */*.ui */*.h */*.cpp
			    */*/*.ui */*/*.h */*/*.cpp)

qt5_add_resources(PROJECT_SOURCES res_rls_mr231.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RLS-MR231
	MANUAL_FINALIZATION
	${PROJECT_SOURCES}
    )
else()
	add_executable(RLS-MR231
	    ${PROJECT_SOURCES}
	)
endif()

########################    PRJ LIBS	########################

add_subdirectory(prj_version_lib/)
target_link_libraries(RLS-MR231 PRIVATE prjversionlib)

if(DEFINED PRJ_COMMON_DIR)
    include_directories(${PRJ_COMMON_DIR})
endif()

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
if(is_top_level)

    set(PRJ_COMMON_DIR ../Libs/DB)
    target_include_directories(RLS-MR231 PRIVATE ${PRJ_COMMON_DIR}/common)

    macro(AddPackageMacros)
	if (NOT ${ARGV0})
	    add_subdirectory(${PRJ_COMMON_DIR}/${ARGV1} ${ARGV1})
	endif()
	find_package(${ARGV1})
    endmacro()

    AddPackageMacros(LOGGING_FOUND logging)
    AddPackageMacros(SETTINGS_FOUND settings)
    AddPackageMacros(DATAMODEL_FOUND datamodel)
    AddPackageMacros(DATAWIDGET_FOUND datawidget)
    AddPackageMacros(CONTROLLERLIB_FOUND controllerlib)
    AddPackageMacros(DBHELPER_SITUATION_FOUND dbhelper_situation)
    AddPackageMacros(DBHELPER_SITUATION_FOUND dbhelperpsql)
    AddPackageMacros(DBHELPER_SITUATION_FOUND dbhelper)

endif()

target_link_libraries(RLS-MR231 PRIVATE logging::logging)
target_link_libraries(RLS-MR231 PRIVATE settings::settings)
target_link_libraries(RLS-MR231 PRIVATE datamodel::datamodel)
target_link_libraries(RLS-MR231 PRIVATE datawidget::datawidget)
target_link_libraries(RLS-MR231 PRIVATE controllerlib::controllerlib)
target_link_libraries(RLS-MR231 PRIVATE dbhelper::dbhelper)
target_link_libraries(RLS-MR231 PRIVATE dbhelperpsql::dbhelperpsql)
target_link_libraries(RLS-MR231 PRIVATE dbhelper_situation::dbhelper_situation)

########################################################################

target_link_libraries(RLS-MR231 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(RLS-MR231 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RLS-MR231)
endif()
