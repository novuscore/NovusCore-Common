project(npas4 VERSION 1.0.0)

file(GLOB_RECURSE NPAS4_FILES "*.cpp" "*.h")

add_library(${PROJECT_NAME} ${NPAS4_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

add_compile_definitions(NPAS4_STATIC)
target_compile_definitions(${PROJECT_NAME} INTERFACE NPAS4_STATIC)
target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")