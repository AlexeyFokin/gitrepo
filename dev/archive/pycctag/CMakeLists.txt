cmake_minimum_required(VERSION 3.5.1)
project(pycctag) 

set(CMAKE_CXX_STANDARD 14)

# +++++++++++++ CCTag 3rd party ++++++++++++++++++++++++++++
find_package(CCTag CONFIG REQUIRED
	     PATHS /home/alexey/dev/CCTag/build/src/generated
	     NO_DEFAULT_PATH#
)# set the path includes CCTagConfig.cmake


# +++++++++++++ pybind11 ++++++++++++++++++++++++++++
#add_subdirectory(pybind11-2.12.0) #cmakelists.txt and pybind11-2.5.0(or another version) folder are in same directory

# /home/alexey/.virtualenvs/ml/lib/python3.10/site-packages
find_package(pybind11 REQUIRED)  # Adjust the version requirement if needed

# if(NOT PYBIND11_FOUND)
#   message(FATAL_ERROR "pybind11 not found")
# endif()

# Link your target with pybind11
#target_link_libraries(pybind11_add_module PRIVATE pybind11::pybind11)

# ++++++++++++++ adding files to target ++++++++++++++++++++++
pybind11_add_module(pycctag pycctag.cpp)
target_include_directories(pycctag PUBLIC ${CCTAG_INSTALL}/include/cctag)
target_link_libraries(pycctag PUBLIC CCTag)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)  # Add the include directory

SET( CMAKE_CXX_FLAGS "-std=c++11 -O3")
