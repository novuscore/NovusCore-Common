project(stormlib VERSION 1.0.0)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set(STORMLIB_SOURCE
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${STORMLIB_SOURCE}
)

source_group(StormLib ${STORMLIB_SOURCE})
target_link_libraries(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_LIST_DIR}/lib/stormlib.lib")
# RELEASE LIBS
target_link_libraries(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_LIST_DIR}/lib/stormlibRAD.lib" "${CMAKE_CURRENT_LIST_DIR}/lib/stormlibRAS.lib" "${CMAKE_CURRENT_LIST_DIR}/lib/stormlibRUD.lib" "${CMAKE_CURRENT_LIST_DIR}/lib/stormlibRUS.lib")
# DEBUG LIBS
target_link_libraries(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_LIST_DIR}/lib/stormlibDAD.lib" "${CMAKE_CURRENT_LIST_DIR}/lib/stormlibDAS.lib" "${CMAKE_CURRENT_LIST_DIR}/lib/stormlibDUD.lib" "${CMAKE_CURRENT_LIST_DIR}/lib/stormlibDUS.lib")

