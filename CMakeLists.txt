cmake_minimum_required(VERSION 3.29)

project(petitechose_midi_studio_plugin_bitwig LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/MidiStudioBitwigSources.cmake")

add_library(midi_studio_plugin_bitwig_sources INTERFACE)
target_sources(midi_studio_plugin_bitwig_sources
    INTERFACE ${MS_PLUGIN_BITWIG_SOURCES})
target_include_directories(midi_studio_plugin_bitwig_sources
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(midi_studio_plugin_bitwig_test_headers INTERFACE)
target_include_directories(midi_studio_plugin_bitwig_test_headers
    INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(MS_PLUGIN_BITWIG_BUILD_TESTS_DEFAULT OFF)
if(PROJECT_IS_TOP_LEVEL)
    set(MS_PLUGIN_BITWIG_BUILD_TESTS_DEFAULT ON)
endif()

option(
    MS_PLUGIN_BITWIG_BUILD_TESTS
    "Build and register MIDI Studio Bitwig plugin unit tests"
    ${MS_PLUGIN_BITWIG_BUILD_TESTS_DEFAULT})

set(MS_PLUGIN_BITWIG_OPEN_CONTROL_ROOT
    "${CMAKE_CURRENT_SOURCE_DIR}/../../open-control"
    CACHE PATH "Path to the local open-control workspace")

if(MS_PLUGIN_BITWIG_BUILD_TESTS AND BUILD_TESTING)
    # Tests that exercise reactive state compile the framework primitives that
    # Signal.hpp needs. The workspace keeps open-control beside midi-studio.
    set(MS_PLUGIN_BITWIG_FRAMEWORK_TESTS
        test_RemoteControlParameterUpdates)
    set(MS_PLUGIN_BITWIG_TEST_FRAMEWORK_DIR
        "${MS_PLUGIN_BITWIG_OPEN_CONTROL_ROOT}/framework")
    set(MS_PLUGIN_BITWIG_TEST_FRAMEWORK_SOURCES
        "${MS_PLUGIN_BITWIG_TEST_FRAMEWORK_DIR}/src/oc/state/Signal.cpp"
        "${MS_PLUGIN_BITWIG_TEST_FRAMEWORK_DIR}/src/oc/state/NotificationQueue.cpp")

    file(GLOB MS_PLUGIN_BITWIG_TESTS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/test/test_*/test_main.cpp")

    set(MS_PLUGIN_BITWIG_TEST_TARGETS)

    foreach(test_source IN LISTS MS_PLUGIN_BITWIG_TESTS)
        get_filename_component(test_dir "${test_source}" DIRECTORY)
        get_filename_component(test_name "${test_dir}" NAME)
        list(APPEND MS_PLUGIN_BITWIG_TEST_TARGETS "${test_name}")

        add_executable("${test_name}" "${test_source}")
        target_link_libraries("${test_name}" PRIVATE midi_studio_plugin_bitwig_test_headers)
        target_include_directories("${test_name}"
            PRIVATE
                "${CMAKE_CURRENT_SOURCE_DIR}/test"
                "${CMAKE_CURRENT_SOURCE_DIR}/src")

        if(test_name IN_LIST MS_PLUGIN_BITWIG_FRAMEWORK_TESTS)
            if(NOT EXISTS "${MS_PLUGIN_BITWIG_TEST_FRAMEWORK_DIR}/src/oc/state/Signal.cpp")
                message(FATAL_ERROR
                    "OpenControl framework not found at "
                    "${MS_PLUGIN_BITWIG_TEST_FRAMEWORK_DIR}")
            endif()
            target_sources("${test_name}" PRIVATE ${MS_PLUGIN_BITWIG_TEST_FRAMEWORK_SOURCES})
            target_include_directories("${test_name}"
                PRIVATE "${MS_PLUGIN_BITWIG_TEST_FRAMEWORK_DIR}/src")
        endif()

        add_test(NAME "${test_name}" COMMAND "${test_name}")
        set_tests_properties("${test_name}" PROPERTIES LABELS plugin-bitwig)
    endforeach()

    if(PROJECT_IS_TOP_LEVEL)
        add_custom_target(test-native
            COMMAND "${CMAKE_CTEST_COMMAND}" --test-dir "${CMAKE_BINARY_DIR}" --parallel --output-on-failure
            DEPENDS ${MS_PLUGIN_BITWIG_TEST_TARGETS}
            USES_TERMINAL)
    endif()
endif()
