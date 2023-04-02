
set(BUILD_LIB_NAME someipservice)
set(some_ip_path $ENV{SOMEIP_INSTALLATION_PATH})


message(${some_ip_path})
message(${some_ip_path})
message(${some_ip_path})
message(${some_ip_path})
message(${some_ip_path})
message(${some_ip_path})
message(${some_ip_path})
message(${some_ip_path})
message(${some_ip_path})
message(${some_ip_path})


find_library(vsomeip3_lib vsomeip3 
            NAMES vsomeip vsomeip3
            REQUIRED
            PATHS ${some_ip_path}/build
            NO_DEFAULT_PATH)

find_package( Boost 1.55 COMPONENTS system thread log REQUIRED )

include_directories(
    /home/abbas/grad_proj_repos/sand_box/vsomeip_containers/vsomeip-3.1.20.3/interface
)

add_library(${BUILD_LIB_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/ServiceManagerAdapter.cpp)

target_link_libraries(${BUILD_LIB_NAME} PUBLIC ${vsomeip3_lib} ${Boost_LIBRARIES})

