cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(GreeterTests LANGUAGES CXX)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage("gh:onqtam/doctest#2.4.5")
CPMAddPackage("gh:TheLartians/Format.cmake@1.7.0")

if(TEST_INSTALLED_VERSION)
  find_package(GreeterMainProject REQUIRED)
else()
  CPMAddPackage(NAME GreeterMainProject SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} doctest::doctest)
target_link_libraries(${PROJECT_NAME} GreeterMainProject::GreeterMainProject)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wpedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# ---- Add GreeterTests ----

enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME ${PROJECT_NAME} COMMAND
# ${PROJECT_NAME})

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(${PROJECT_NAME})

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(${PROJECT_NAME} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(${PROJECT_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
  include(../cmake/CodeCoverage.cmake)
  add_compile_options(-O0 -g)
  setup_target_for_coverage_gcovr_xml(
    NAME
    test_coverage # New target name
    EXECUTABLE
    ${PROJECT_NAME}
    -j
    ${PROCESSOR_COUNT} # Executable in PROJECT_BINARY_DIR
    # DEPENDENCIES testrunner # Dependencies to build first
    BASE_DIRECTORY
    "../" # Base directory for report (defaults to PROJECT_SOURCE_DIR)
    # EXCLUDE "src/dir1/*" "src/dir2/*" # Patterns to exclude (can be relative to BASE_DIRECTORY,
    # with CMake 3.4+) NO_DEMANGLE # Don't demangle C++ symbols even if c++filt is found
  )

endif()
