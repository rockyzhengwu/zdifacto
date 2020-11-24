cmake_minimum_required(VERSION 3.13)

project(zdifacto VERSION 1.0)

if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
  message(FATAL_ERROR "GCC version must be at least 5.0!")
endif()

set(${CMAKE_CXX_FLAGS}, "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -fPIC -O3 -ggdb -Wall -DDMLC_LOG_FATAL_THROW=0 $(ADD_CFLAGS)")
# find_package(OpenMP REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/dmlc-core)
add_subdirectory(${PROJECT_SOURCE_DIR}/ps-lite)

set_target_properties(dmlc PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  POSITION_INDEPENDENT_CODE ON
)

file(GLOB_RECURSE SOURCES 
   ${PROJECT_SOURCE_DIR}/src/updater*.h
   ${PROJECT_SOURCE_DIR}/src/updater/*.cc
   ${PROJECT_SOURCE_DIR}/src/reader/*.h
   ${PROJECT_SOURCE_DIR}/src/reader/*.cc
   ${PROJECT_SOURCE_DIR}/src/sgd/*.h
   ${PROJECT_SOURCE_DIR}/src/sgd/*.cc
   ${PROJECT_SOURCE_DIR}/src/store/*.h
   ${PROJECT_SOURCE_DIR}/src/store/*.cc
   ${PROJECT_SOURCE_DIR}/src/tracker/*.h
   ${PROJECT_SOURCE_DIR}/src/tracker/*.cc
   ${PROJECT_SOURCE_DIR}/src/data/*.h
   ${PROJECT_SOURCE_DIR}/src/data/*.cc
   ${PROJECT_SOURCE_DIR}/src/common/*.h
   ${PROJECT_SOURCE_DIR}/src/loss/*.h
   ${PROJECT_SOURCE_DIR}/src/loss/*.cc
   ${PROJECT_SOURCE_DIR}/src/predicter/*.h
   ${PROJECT_SOURCE_DIR}/src/predicter/*.cc
   ${PROJECT_SOURCE_DIR}/src/learner.cc
   ${PROJECT_SOURCE_DIR}/src/updater.cc
   ${PROJECT_SOURCE_DIR}/src/predicter.cc
)
include(GNUInstallDirs)
message(${PROJECT_SOURCE_DIR})

add_library(zdifacto "")



target_include_directories(zdifacto
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/dmlc-core/include
  ${PROJECT_SOURCE_DIR}/dmlc-core/src
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/ps-lite/include
)
target_sources(zdifacto PRIVATE ${SOURCES})

target_link_libraries(zdifacto PUBLIC dmlc)
target_link_libraries(zdifacto PUBLIC pslite)
find_package(OpenMP REQUIRED)
target_link_libraries(zdifacto PUBLIC OpenMP::OpenMP_CXX)

add_executable(zdifacto_train src/main.cc )

target_include_directories(zdifacto_train
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/dmlc-core/include
  ${PROJECT_SOURCE_DIR}/dmlc-core/src
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/ps-lite/include})

target_link_libraries(zdifacto_train zdifacto)
