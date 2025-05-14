cmake_minimum_required(VERSION 3.13)

project(LightLanguageProcessor)

option(LLP_SHARED "Build using shared libraries" ON)

#Global cmake features
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


file(GLOB_RECURSE SOURCES src/**)
file(GLOB_RECURSE PUB_HEADERS include/**)

if ($(LLP_SHARED))
    add_library(llp SHARED ${SOURCES})
else()
    add_library(llp STATIC ${SOURCES})
endif()

set_property(TARGET llp PROPERTY CXX_STANDARD 20)
set_target_properties(llp PROPERTIES CXX_EXTENSIONS OFF)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES  ${SOURCES})
target_include_directories(llp PRIVATE src)
target_include_directories(llp PUBLIC include)
target_sources(llp PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS include
        FILES ${PUB_HEADERS})

file(GLOB_RECURSE SOURCES tests/**.cpp)
add_executable(llp_tests ${SOURCES})
set_property(TARGET llp_tests PROPERTY CXX_STANDARD 20)
set_target_properties(llp_tests PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(llp_tests PRIVATE tests)
target_link_libraries(llp_tests PRIVATE llp)

install(TARGETS llp FILE_SET public_headers)