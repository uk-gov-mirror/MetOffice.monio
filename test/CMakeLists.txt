###############################################################################
# MONIO - Met Office NetCDF Input Output                                      #
#                                                                             #
# (C) Crown Copyright 2023, Met Office. All rights reserved.                  #
#                                                                             #
# This software is licensed under the terms of the 3-Clause BSD License       #
# which can be obtained from https://opensource.org/license/bsd-3-clause/.    #
###############################################################################

message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

ecbuild_add_resources(TARGET monio_test_scripts SOURCES_PACK ${monio_test_input})

if(NOT DEFINED MONIO_TESTFILES_DIR)
  if(DEFINED ENV{MONIO_TESTFILES_DIR})
    set(MONIO_TESTFILES_DIR "$ENV{MONIO_TESTFILES_DIR}")
  elseif(DEFINED ENV{LFRICLITE_DATA_REP_DIR})  # back compat
    set(MONIO_TESTFILES_DIR "$ENV{LFRICLITE_DATA_REP_DIR}")
  elseif(DEFINED MOBB_DATA)
    set(MONIO_TESTFILES_DIR "${MOBB_DATA}/monio")
  elseif(DEFINED ENV{MOBB_DATA})
    set(MONIO_TESTFILES_DIR "$ENV{MOBB_DATA}/monio")
  endif()
endif()
if(IS_DIRECTORY "${MONIO_TESTFILES_DIR}/Data")
  set(MONIO_TESTFILES_DIR "${MONIO_TESTFILES_DIR}/Data")
endif()
if(IS_DIRECTORY "${MONIO_TESTFILES_DIR}")
  message(STATUS "MONIO_TESTFILES_DIR=${MONIO_TESTFILES_DIR}")
  if(IS_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Data")
    file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/Data")
  endif()
  file(
    CREATE_LINK
    "${MONIO_TESTFILES_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}/Data"
    SYMBOLIC
  )
endif()
# else()
#   message(WARNING
#     "MONIO_TESTFILES_DIR=${MONIO_TESTFILES_DIR}: no such directory: \
#     Skipping tests that require test files.")
#   return()
# endif()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DataOut)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testinput)
list(APPEND monio_testinput
  testinput/fieldset_write.yaml
  testinput/state_basic.yaml
  testinput/state_full.yaml
)

foreach(FILENAME ${monio_testinput})
  file(
    CREATE_LINK
    ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
    ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
    SYMBOLIC
  )
endforeach()

# We can make use of CMake's native support of cpplint in future. If we set
# `CMAKE_<LANG>_CPPLINT` (where `<LANG>` is either `C` or `CXX`) to point to
# the `cpplint` executable, it will run `cpplint` as part of the build.
find_program(
  CPPLINT_EXECUTABLE
  NAMES cpplint
  DOC "Path to the cpplint executable"
)
if(CPPLINT_EXECUTABLE)
  ecbuild_add_test(
    TARGET "${PROJECT_NAME}_coding_norms"
    LABELS lint
    TYPE SCRIPT
    COMMAND "${CPPLINT_EXECUTABLE}"
    ARGS --quiet --recursive "${PROJECT_SOURCE_DIR}"
  )
endif()

if(NOT IS_DIRECTORY "${MONIO_TESTFILES_DIR}")
  message(WARNING
    "MONIO_TESTFILES_DIR=${MONIO_TESTFILES_DIR}: no such directory.\
    Skipping tests that require test files.")
  return()
endif()

ecbuild_add_test(
  TARGET  test_monio_fieldset_write
  SOURCES mains/TestFieldSetWrite.cc
  ARGS    "testinput/fieldset_write.yaml"
  LIBS    monio
  MPI     4)

ecbuild_add_test(
  TARGET  test_monio_state_basic
  SOURCES mains/TestStateBasic.cc
  ARGS    "testinput/state_basic.yaml"
  LIBS    monio
  MPI     4)

ecbuild_add_test(
  TARGET  test_monio_state_full
  SOURCES mains/TestStateFull.cc
  ARGS    "testinput/state_full.yaml"
  LIBS    monio
  MPI     4)
