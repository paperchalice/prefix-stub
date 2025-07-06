cmake_minimum_required(VERSION 4.0)

project(
  prefix-stub
  VERSION 0.0.0
  DESCRIPTION "Detect prefix on Windows"
  LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

set(CMAKE_EXPERIMENTAL_EXPORT_PACKAGE_INFO b80be207-778e-46ba-8080-b23bba22639e)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

add_library(prefix-stub
  SHARED
  relocate.c
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
