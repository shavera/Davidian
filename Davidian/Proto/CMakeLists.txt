cmake_minimum_required (VERSION 3.5)
project (davidian_proto)

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

set(_GRPC_GRPCPP_UNSECURE gRPC::grpc++_unsecure)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

#set(PROTO_FILES Engine.proto Orbital.proto)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS Engine.proto Orbital.proto)

include_directories(${Protobuf_INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${PROTO_SRCS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Protobuf_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

#set(OUTPUT_FILES
#        "${CMAKE_CURRENT_BINARY_DIR}/Engine.grpc.pb.cc"
#        "${CMAKE_CURRENT_BINARY_DIR}/Engine.grpc.pb.h"
#        )
#
#add_custom_command(
#        OUTPUT ${OUTPUT_FILES}
#        COMMAND ${_PROTOBUF_PROTOC}
#        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
#        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
#        -I ${CMAKE_CURRENT_SOURCE_DIR}
#        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
#        ${PROTO_FILES}
#        DEPENDS "${PROTO_FILES}")

#target_link_libraries(${PROJECT_NAME} PUBLIC ${Protobuf_LIBRARIES} ${_GRPC_GRPCPP_UNSECURE})
