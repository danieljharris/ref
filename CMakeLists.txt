cmake_minimum_required(VERSION 3.21)
project(ref VERSION 0.1.0 LANGUAGES CXX)

add_library(ref INTERFACE)
add_library(ref::ref ALIAS ref)

target_include_directories(ref INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(ref INTERFACE cxx_std_23)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(DIRECTORY include/ref DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS ref EXPORT refTargets)

install(EXPORT refTargets
    FILE refTargets.cmake
    NAMESPACE ref::
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/ref")

configure_package_config_file(
    cmake/refConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/refConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/ref"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/refConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/refConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/refConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/ref")