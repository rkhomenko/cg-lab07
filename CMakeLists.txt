cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(cg-lab07 VERSION 0.1.0 LANGUAGES C CXX)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
   "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -isystem")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -DDEBUG")
else()
    # Realization of MSVC compiler support
    message(FATAL_ERROR "You are seriously, man? Use GNU or Clang compiler or go away!")
endif()

set(HEADER_SUFFIX "hpp")
set(SOURCE_SUFFIX "cpp")
set(INCLUDE_DIR "include")
set(SOURCE_DIR "src")
set(RESOURCES_FILE "resources/resources.qrc")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

file(GLOB_RECURSE INCLUDES "${INCLUDE_DIR}/*.${HEADER_SUFFIX}")
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.${SOURCE_SUFFIX}")

include_directories(${INCLUDE_DIR})
include_directories(${Qt5Widgets_INCLUDE_DIRS})

qt5_add_resources(RESOURCES ${RESOURCES_FILE})

find_package(OpenGL REQUIRED)

if(EIGEN3_INCLUDE_DIR)
    add_definitions(-DEIGEN3_INCLUDE_DIR)
    include_directories(${EIGEN3_INCLUDE_DIR})
endif()

add_executable(${PROJECT_NAME} ${INCLUDES} ${UI_INCLUDES}
                               ${SOURCES}
                               ${RESOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
