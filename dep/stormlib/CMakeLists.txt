project(stormlib VERSION 1.0.0)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set(STORMLIB_SOURCE
    "${CMAKE_CURRENT_LIST_DIR}/include"
    "${CMAKE_CURRENT_LIST_DIR}/lib"

)

target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${STORMLIB_SOURCE}
)

file(GLOB STORMLIB_LIBS "${CMAKE_CURRENT_LIST_DIR}/lib/*.lib")
source_group(StormLib ${STORMLIB_SOURCE})
target_link_libraries(${PROJECT_NAME} INTERFACE "${STORMLIB_LIBS}")

