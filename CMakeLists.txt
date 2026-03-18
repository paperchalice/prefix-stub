cmake_minimum_required(VERSION 4.3)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD 451f2fe2-a8a2-47c3-bc32-94786d8fc91b)

project(
  prefix-stub
  VERSION 0.0.0
  DESCRIPTION "Detect prefix on Windows"
  LANGUAGES C CXX
)

set(CMAKE_C_STANDARD ${CMAKE_C_STANDARD_LATEST})
set(CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD_LATEST})
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_CXX_MODULE_STD ON)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

add_library(prefix-stub
  SHARED
  relocate.cpp
  app.manifest
)
generate_export_header(prefix-stub
  EXPORT_FILE_NAME
    prefix-stub/export.h
)
target_compile_options(prefix-stub
  PRIVATE
    $<$<AND:$<COMPILE_LANGUAGE:C>,$<C_COMPILER_FRONTEND_VARIANT:MSVC>>:-Zc:inline;-Zc:preprocessor>
)
target_sources(
  prefix-stub
  PUBLIC
    FILE_SET HEADERS
      FILES prefix-stub/relocate.h
    FILE_SET generated_headers
      TYPE HEADERS
      BASE_DIRS $<TARGET_PROPERTY:prefix-stub,BINARY_DIR>
      FILES ${CMAKE_CURRENT_BINARY_DIR}/prefix-stub/export.h
)

install(
  TARGETS prefix-stub
  EXPORT prefix-stub-targets
  FILE_SET HEADERS
  FILE_SET generated_headers
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(prefix-stub-config.cmake.in
  prefix-stub-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
configure_file(prefix-stub.pc.in prefix-stub.pc @ONLY)

install(EXPORT
  prefix-stub-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  NAMESPACE me::
)
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/prefix-stub-config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/prefix-stub.pc"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
install(PACKAGE_INFO prefix-stub EXPORT prefix-stub-targets
  # PROJECT ${PROJECT_NAME}
)
