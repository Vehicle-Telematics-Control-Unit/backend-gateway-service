cmake_minimum_required(VERSION 3.0.0)
project(backend_gateway VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/http_client)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/someipService)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(${PROJECT_NAME} http_client someipservice)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies
    /home/abbas/grad_proj_repos/sand_box/vsomeip_containers/vsomeip-3.1.20.3/interface
    )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
