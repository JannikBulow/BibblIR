cmake_minimum_required(VERSION 3.26)
project(BibblIR VERSION 0.1.0 LANGUAGES CXX)

include(CMakePackageConfigHelpers)

add_subdirectory(framework)
add_subdirectory(main)

configure_package_config_file(
    cmake/BibblIRConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/BibblIRConfig.cmake
    INSTALL_DESTINATION lib/cmake/BibblIR
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/BibblIRConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/BibblIRConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/BibblIRConfigVersion.cmake
    DESTINATION lib/cmake/BibblIR
)
