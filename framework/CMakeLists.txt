cmake_minimum_required(VERSION 3.26)

include(FetchContent)
include(GenerateExportHeader)

FetchContent_Declare(BibbleASM
    GIT_REPOSITORY https://github.com/JannikBulow/BibbleASM
    GIT_TAG master
)
FetchContent_MakeAvailable(BibbleASM)

set(SOURCES
    src/ir/basicblock.cpp

    src/type/type.cpp

    src/module.cpp
    src/type/types.cpp
)

set(HEADERS
    include/BibblIR/ir/basicblock.h
    include/BibblIR/ir/value.h

    include/BibblIR/type/integer_type.h
    include/BibblIR/type/type.h
    include/BibblIR/type/void_type.h

    include/BibblIR/visitor/visitor.h

    include/BibblIR/module.h
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(BibblIR-framework-shared SHARED ${SOURCES} ${HEADERS})
add_library(BibblIR-framework-static STATIC ${SOURCES} ${HEADERS})

generate_export_header(BibblIR-framework-shared
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/BibblIR/api.h
    EXPORT_MACRO_NAME BIBBLIR_EXPORT
    NO_EXPORT_MACRO_NAME BIBBLIR_NO_EXPORT
    STATIC_DEFINE BIBBLIR_STATIC_DEFINE
)

target_compile_definitions(BibblIR-framework-static PUBLIC BIBBLIR_STATIC_DEFINE)

target_include_directories(BibblIR-framework-shared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_include_directories(BibblIR-framework-static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(BibblIR-framework-shared PUBLIC cxx_std_20)
target_compile_features(BibblIR-framework-static PUBLIC cxx_std_20)

target_link_libraries(BibblIR-framework-shared BibbleASM-framework-shared)
target_link_libraries(BibblIR-framework-static BibbleASM-framework-static)

set_target_properties(BibblIR-framework-shared PROPERTIES OUTPUT_NAME "bibblir")
set_target_properties(BibblIR-framework-static PROPERTIES OUTPUT_NAME "bibblir-aio")
