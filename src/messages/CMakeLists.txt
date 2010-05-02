

add_library(messages motion.pb.cc TestMessage.pb.cc VisionObservations.pb.cc VisionObservations.pb.h SensorsMessage.pb.h SensorsMessage.pb.cc Gamecontroller.pb.cc)
target_link_libraries(messages ${PROTOBUF_LIBRARIES})
