cmake_minimum_required(VERSION 3.8)

project(zdifacto VERSION 1.0)

# include_directories(
#   ${PROJECT_SOURCE_DIR}
#   ${PROJECT_BINARY_DIR}
#   ${PROJECT_SOURCE_DIR}/include
#   ${PROJECT_SOURCE_DIR}/dmlc-core/include
#   ${PROJECT_SOURCE_DIR}/ps-lite/include
# )


# CFLAGS = -std=c++11 -fopenmp -fPIC -O3 -ggdb -Wall -DDMLC_LOG_FATAL_THROW=0 $(ADD_CFLAGS)
set(${CMAKE_CXX_FLAGS}, "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -fPIC -O3 -ggdb -Wall -DDMLC_LOG_FATAL_THROW=0 $(ADD_CFLAGS)")
# find_package(OpenMP REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/dmlc-core)
add_subdirectory(${PROJECT_SOURCE_DIR}/ps-lite)

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
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

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
# target_link_libraries(zdifacto PUBLIC OpenMP::OpenMP_CXX)

# target_compile_options(zdifacto
#   PRIVATE
#   $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<COMPILE_LANGUAGE:CXX>>:/MP>
#   $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<COMPILE_LANGUAGE:CXX>>:-funroll-loops>)

add_executable(zdifacto_train src/main.cc )

target_include_directories(zdifacto_train
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/dmlc-core/include
  ${PROJECT_SOURCE_DIR}/dmlc-core/src
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/ps-lite/include})

target_link_libraries(zdifacto_train zdifacto)
