file(GLOB HEADERS "*.hpp"
        "*.h")

include_directories("include/ftllib")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../libc++/libcxx/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../libc/musl/include")

add_library(ftl_malloc
        src/malloc.cpp
        ${HEADERS})

add_library(ftl_cmem
        src/memory.cpp
        ${HEADERS})

set_target_properties(ftl_malloc PROPERTIES LINKER_LANGUAGE C)

INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../ftllib/include/ DESTINATION ${BASE_BINARY_DIR}/include/)
INSTALL(TARGETS ftl_malloc DESTINATION ${BASE_BINARY_DIR}/lib/)
INSTALL(TARGETS ftl_cmem DESTINATION ${BASE_BINARY_DIR}/lib/)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ftl.imports DESTINATION ${BASE_BINARY_DIR}/lib/)
