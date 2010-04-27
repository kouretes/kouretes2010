

add_library(messages motion.pb.cc TestMessage.pb.cc)
target_link_libraries(messages ${PROTOBUF_LIBRARIES})
