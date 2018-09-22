cmake_minimum_required(VERSION 3.12)
project(formal_patch)

set(CMAKE_CXX_STANDARD 17)


find_library(LLVM NAMES llvm LLVM HINTS /usr/lib)

add_executable(formal_patch main.cpp src/atom.h src/result.h src/errors.h)
target_link_libraries(formal_patch LLVM)

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()