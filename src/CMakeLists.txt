project(toytls)

set (PACKAGE_VERSION 0.0.1)

find_package(fmt REQUIRED CONFIG)

aux_source_directory(${LIBTOYTLS_PROJECT_DIR}/src/libtoytls SRC_FILES)
aux_source_directory(${LIBTOYTLS_PROJECT_DIR}/src/libtoytls/util SRC_FILES)
file(GLOB HEAD_FILES
  ${LIBTOYTLS_PROJECT_DIR}/include/libtoytls/*.h
  ${LIBIPC_PROJECT_DIR}/src/libtoytls/*.h
  ${LIBIPC_PROJECT_DIR}/src/libtoytls/util/*.h)

if (LIBTOYTLS_BUILD_SHARED_LIBS)
  add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEAD_FILES})
else()
  add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${HEAD_FILES})
endif()

target_include_directories(${PROJECT_NAME}
  PUBLIC  
    "$<BUILD_INTERFACE:${LIBTOYTLS_PROJECT_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
  PRIVATE ${LIBTOYTLS_PROJECT_DIR}/src)

target_link_libraries(${PROJECT_NAME} 
  PUBLIC 
    fmt::fmt)