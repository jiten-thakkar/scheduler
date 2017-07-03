cmake_minimum_required(VERSION 3.1.0)

project(GenericScheduler)

add_compile_options(-std=c++11)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed -ldl" )

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
  include/TCPTimeTask.h
  lib/Scheduler.cpp
  lib/VirtualMemoryUsageTask.cpp
  lib/TCPTimeTask.cpp
  )

add_library(sqlite-wrapper STATIC IMPORTED) # or STATIC instead of SHARED
set_target_properties(sqlite-wrapper PROPERTIES
  IMPORTED_LOCATION "${SQLITE_WRAPPER_DIR}/lib/libkompex-sqlite-wrapper.a"
  INTERFACE_INCLUDE_DIRECTORIES "${SQLITE_WRAPPER_DIR}/include"
  )

add_executable(example 
  src/main.cpp
  )

find_package(Threads REQUIRED)

target_link_libraries(example Scheduler sqlite-wrapper Threads::Threads sqlite3)
