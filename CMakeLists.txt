cmake_minimum_required(VERSION 3.9.1)
project(esdcan)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
message(${CMAKE_BINARY_DIR})

add_executable(can_receive canreceive.cpp)
add_executable(can_send cansend.cpp)

#target_include_directories(can_receive PUBLIC "/")

target_link_libraries(can_receive ntcan)
target_link_libraries(can_send ntcan)


install(TARGETS can_receive
    DESTINATION ${EXECUTABLE_OUTPUT_PATH}
    )
install(TARGETS can_receive
    DESTINATION ${EXECUTABLE_OUTPUT_PATH}
    )
