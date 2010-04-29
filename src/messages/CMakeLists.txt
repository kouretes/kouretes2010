

add_library(messages motion.pb.cc TestMessage.pb.cc VisionObservations.pb.cc VisionObservations.pb.h)
target_link_libraries(messages ${PROTOBUF_LIBRARIES})
