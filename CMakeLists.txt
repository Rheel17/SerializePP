cmake_minimum_required(VERSION 3.16)
project(SerializePP VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include(GNUInstallDirs)

add_library(SerializePP INTERFACE)

target_include_directories(SerializePP
  INTERFACE $<BUILD_INTERFACE:${SerializePP_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(SerializePP INTERFACE cxx_std_20)

install(TARGETS SerializePP
        EXPORT SerializePP_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("SerializePPConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/SerializePPconfig.cmake.in"
  "${PROJECT_BINARY_DIR}/SerializePPConfig.cmake"
  INSTALL_DESTINATION
  ${CMAKE_INSTALL_DATAROOTDIR}/SerializePP/cmake)

install(EXPORT SerializePP_Targets
        FILE SerializePPTargets.cmake
        NAMESPACE SerializePP::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/SerializePP/cmake)

install(FILES "${PROJECT_BINARY_DIR}/SerializePPConfig.cmake"
              "${PROJECT_BINARY_DIR}/SerializePPConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/SerializePP/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/serializepp DESTINATION include)

