cmake_minimum_required(VERSION 2.8)

project(GenericScheduler)

add_compile_options(-std=c++11)

set(SQLITE_WRAPPER_DIR ${CMAKE_SOURCE_DIR}/dependency/kompex-sqlite-wrapper)

add_custom_command(
  OUTPUT ${SQLITE_WRAPPER_DIR}/lib/libkompex-sqlite-wrapper.so
  WORKING_DIR ${CMAKE_SOURCE_DIR}/dependency/kompex-sqlite-wrapper
  COMMAND ./configure && make && make install
  )

include_directories(include
  ${SQLITE_WRAPPER_DIR}/include
  )

add_library(Scheduler STATIC
  include/Scheduler.h
  include/Schedule.h
  include/Task.h
  include/VirtualMemoryUsageTask.h
  lib/Scheduler.cpp
  lib/VirtualMemoryUsageTask.cpp
  )

add_library(sqlite-wrapper STATIC IMPORTED) # or STATIC instead of SHARED
set_target_properties(sqlite-wrapper PROPERTIES
  IMPORTED_LOCATION "${SQLITE_WRAPPER_DIR}/lib/libkompex-sqlite-wrapper.so"
  INTERFACE_INCLUDE_DIRECTORIES "${SQLITE_WRAPPER_DIR}/include"
  )


add_executable(example 
  src/main.cpp
  )

find_library(SQLITE_WRAPPER libkompex-sqlite-wrapper HINTS ${SQLITE_WRAPPER_DIR}/lib)
target_link_libraries(example Scheduler sqlite-wrapper)
