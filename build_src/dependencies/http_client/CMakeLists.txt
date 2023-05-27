set(BUILD_LIB_NAME http_client)
cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)

find_library(curl_lib curl NAMES libcurl)

if(${curl_lib} STREQUAL curl_lib-NOTFOUND)
    message("NOT FOUND")
else()
    message(${curl_lib})
endif()


add_library(${BUILD_LIB_NAME} SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/GetRequestClient.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HttpRequest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HttpRequestBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PostRequestClient.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PutRequestClient.cpp
)

include_directories(
    ${CURL_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(${BUILD_LIB_NAME} PUBLIC ${curl_lib})

