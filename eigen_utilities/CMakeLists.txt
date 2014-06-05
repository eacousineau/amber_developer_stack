cmake_minimum_required(VERSION 2.8.3)
project(eigen_utilities)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  common_assert
  common_cmake
)

## System dependencies are found with CMake's conventions
find_package(eigen3 REQUIRED)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
 INCLUDE_DIRS
   include
   ${EIGEN3_INCLUDE_DIR}
  CATKIN_DEPENDS
    common_assert
  DEPENDS eigen3
)

###########
## Build ##
###########

include(common_flags)
common_add_vectorization_flags()

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(include)
# TODO: Check names of system library include directories (eigen3)
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

add_library(${PROJECT_NAME}
  src/runtime_config.cpp
)

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
catkin_add_gtest(${PROJECT_NAME}-test test/test_runtime_config.cpp)
if(TARGET ${PROJECT_NAME}-test)
  target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
endif()
