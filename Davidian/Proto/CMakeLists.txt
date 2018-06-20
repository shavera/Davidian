cmake_minimum_required (VERSION 3.5)
project (davidian_proto)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS Engine.proto)

include_directories(${Protobuf_INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${PROTO_SRCS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Protobuf_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

