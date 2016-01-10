cmake_minimum_required(VERSION "3.3.0" FATAL_ERROR)

# Must be before PROJECT keyword
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(Fast-Fs CXX)

enable_testing()

include_directories(${PROJECT_SOURCE_DIR}/source)
include_directories(SYSTEM tests/gmock/include tests/gmock/gtest/include)
link_directories(lib)

add_compile_options(-g -O0 -Weffc++ -std=gnu++14 -Wall -Wextra -Wno-missing-field-initializers -Wno-unused-local-typedefs -Wno-unused-parameter -Wno-deprecated-declarations -Wshadow -fno-omit-frame-pointer -Woverloaded-virtual -Wcast-align)

set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} " -Wl,-static -static-libgcc")

file( GLOB SOURCES RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/source/*.cpp )
file( GLOB HEADERS RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/source/*.h )
file( GLOB UNIT_TESTS RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/tests/*_unittest.cpp )

add_library( Production STATIC ${SOURCES} )

foreach( testsourcefile ${UNIT_TESTS} )
  get_filename_component( testexecutable "${testsourcefile}" NAME_WE )
  add_executable( ${testexecutable} ${testsourcefile} )
  target_link_libraries( ${testexecutable} Production ${PROJECT_SOURCE_DIR}/tests/libs/win/libgmock.a )
#  add_test( NAME ${testexecutable} COMMAND ${testexecutable} )
#  add_custom_command( TARGET ${testexecutable} POST_BUILD COMMAND ${testexecutable} )
  add_custom_command( TARGET ${testexecutable}
                      POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -DTEST=$<TARGET_FILE:${testexecutable}> -DARGS=" --gtest_shuffle --gtest_color=yes" -DRESULT_FILE="$<TARGET_FILE:${testexecutable}>.out" -P ${CMAKE_SOURCE_DIR}/unittest.cmake )
#  add_custom_target( "${testexecutable}.out" DEPENDS ${testexecutable} )
#  add_custom_command( TARGET "${testexecutable}.out"
#                      POST_BUILD
#                      COMMAND ${CMAKE_COMMAND} -DTEST=$<TARGET_FILE:${testexecutable}> -DRESULT_FILE="$<TARGET_FILE:${testexecutable}.out" -P ${CMAKE_SOURCE_DIR}/unittest.cmake )
endforeach( testsourcefile ${UNIT_TESTS} )

# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#  message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
