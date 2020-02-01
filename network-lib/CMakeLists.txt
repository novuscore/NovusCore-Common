project(network VERSION 1.0.0 DESCRIPTION "Network Library")

file(GLOB_RECURSE NETWORK_LIB_FILES "*.cpp" "*.h")

add_library(${PROJECT_NAME} ${NETWORK_LIB_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${ROOT_FOLDER}/libs)

find_assign_files(${NETWORK_LIB_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC
	asio::asio
	common::common
)

add_compile_definitions(NOMINMAX _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)