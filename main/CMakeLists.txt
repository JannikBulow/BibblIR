cmake_minimum_required(VERSION 3.26)

set(SOURCES
    src/main.cpp
)

set(HEADERS
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(BibblIR-main ${SOURCES} ${HEADERS})

target_include_directories(BibblIR-main PUBLIC include)

target_compile_features(BibblIR-main PUBLIC cxx_std_20)

target_link_libraries(BibblIR-main PRIVATE BibblIR-framework-shared)

set_target_properties(BibblIR-main PROPERTIES OUTPUT_NAME "bibblir")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_custom_command(TARGET BibblIR-main PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:BibblIR-framework-shared>
        $<TARGET_FILE_DIR:BibblIR-main>
    )

    add_custom_command(TARGET BibblIR-main PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:BibbleASM-framework-shared>
        $<TARGET_FILE_DIR:BibblIR-main>
    )

    add_custom_command(TARGET BibblIR-main PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:BibbleBytecode-shared>
        $<TARGET_FILE_DIR:BibblIR-main>
    )

    if (MSVC)
        add_custom_command(TARGET BibblIR-main PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_PDB_FILE:BibblIR-framework-shared>
            $<TARGET_PDB_FILE_DIR:BibblIR-main>
        )

        add_custom_command(TARGET BibblIR-main PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_PDB_FILE:BibbleASM-framework-shared>
            $<TARGET_PDB_FILE_DIR:BibblIR-main>
        )

        add_custom_command(TARGET BibblIR-main PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_PDB_FILE:BibbleBytecode-shared>
            $<TARGET_PDB_FILE_DIR:BibblIR-main>
        )
    endif()
endif()