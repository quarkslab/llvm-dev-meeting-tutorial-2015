cmake_minimum_required(VERSION 3.0)

# LLVM detection part
# The user is supposed to set this to a valid llvm3.7 install root
set(LLVM_ROOT "" CACHE PATH "Root of LLVM install.")

# A bit of a sanity check:
if(NOT EXISTS ${LLVM_ROOT}/include/llvm )
    message(FATAL_ERROR
            "LLVM_ROOT (${LLVM_ROOT}) is invalid")
endif()

# Load various LLVM config stuff,
# see http://llvm.org/docs/CMake.html#developing-llvm-passes-out-of-source
list(APPEND CMAKE_PREFIX_PATH
     "${LLVM_ROOT}/share/llvm/cmake")
find_package(LLVM REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions) # load additional config
include(AddLLVM) # used to add our own modules

# propagate LLVM-specific variables to this project
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_CFG_INTDIR})
set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_CFG_INTDIR})

# Following commands are specific to this tutorial

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(Utils)
add_subdirectory(MBA)
add_subdirectory(ReachableIntegerValues)
add_subdirectory(DuplicateBB)

# testing part relies on lit, dependance is required to avoid cmake bloat
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Python)
find_python_module(lit REQUIRED)

# set various configuration settings in the test suite
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Tests/lit.cfg.in" "${CMAKE_CURRENT_BINARY_DIR}/Tests/lit.cfg")

if(NOT EXISTS ${LLVM_ROOT}/bin/FileCheck)
    message(FATAL_ERROR "LLVM wasn't configured with -DLLVM_INSTALL_UTILS, cannot use FileCheck")
endif()

# provide the make check target based on lit
add_custom_target(check
    COMMAND ${PYTHON_EXECUTABLE} -m lit.main
            "${CMAKE_CURRENT_BINARY_DIR}/Tests" -v)
