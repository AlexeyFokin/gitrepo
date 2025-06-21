cmake_minimum_required(VERSION 3.12)

project(opencv-swig-test)

cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)
set(CMAKE_CXX_STANDARD 14)
find_package(SWIG REQUIRED)

# +++++++++++++ CCTag 3rd party ++++++++++++++++++++++++++++
#find_package(CCTag CONFIG REQUIRED
#	     PATHS /home/alexey/dev/CCTag/build/src/generated
#	     NO_DEFAULT_PATH#
#)# set the path includes CCTagConfig.cmake
#find_package(CCTag CONFIG REQUIRED)

# add link to the library
#add_executable(cctagtest mat.hpp)
#target_link_libraries(cctagtest PUBLIC CCTag::CCTag)

include_directories(/usr/include/eigen3)  # Add the include directory
include_directories(/home/alexey/dev/CCTag/src)
include_directories(/home/alexey/dev/CCTag)


include(${SWIG_USE_FILE})

find_package(Python3 COMPONENTS Interpreter Development)
find_package(OpenCV REQUIRED opencv_core)
find_package(Boost 1.69.0 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${Python3_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})

enable_testing()

macro(ADD_TEST_RULES name)
    execute_process(COMMAND "${SWIG_EXECUTABLE}" "-python" "-c++"
        ${CMAKE_SWIG_FLAGS} "-MM" "${CMAKE_CURRENT_SOURCE_DIR}/${name}.i"
        OUTPUT_VARIABLE ${name}_DEPENDENCIES)

    # Remove the first line
    string(REGEX REPLACE "^.+: +\\\\\n +" ""
        ${name}_DEPENDENCIES "${${name}_DEPENDENCIES}")
    # Clean the end of each line
    string(REGEX REPLACE " +(\\\\)?\n" "\n"
        ${name}_DEPENDENCIES "${${name}_DEPENDENCIES}")
    # Clean beginning of each line
    string(REGEX REPLACE "\n +" "\n"
        ${name}_DEPENDENCIES "${${name}_DEPENDENCIES}")
    # clean paths
    string(REGEX REPLACE "\\\\\\\\" "/"
        ${name}_DEPENDENCIES "${${name}_DEPENDENCIES}")
    string(REGEX REPLACE "\n" ";"
        ${name}_DEPENDENCIES "${${name}_DEPENDENCIES}")

    set(SWIG_MODULE_${name}_EXTRA_DEPS ${${name}_DEPENDENCIES})
    set_property(SOURCE ${name}.i PROPERTY CPLUSPLUS ON)
    swig_add_library(${name} LANGUAGE python SOURCES ${name}.i ${name}.hpp)
    swig_link_libraries(${name} "${OpenCV_LIBRARIES}" ${Python3_LIBRARIES})
endmacro(ADD_TEST_RULES)

get_property(_include_directories DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
foreach(_include_directory IN LISTS _include_directories)
    list(APPEND CMAKE_SWIG_FLAGS -I${_include_directory})
endforeach()


SET( CMAKE_CXX_FLAGS "-std=c++11 -O3")
add_test_rules(mat)
