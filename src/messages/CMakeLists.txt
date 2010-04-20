

add_library(messages motion.pb.cc)
target_link_libraries(messages ${PROTOBUF_LIBRARIES})
