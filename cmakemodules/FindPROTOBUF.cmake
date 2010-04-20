message("------------------ Google protocol buffers -------------")

#Act depending on the architecture
include(${KOURETES_DIR}/cmakemodules/${TARGET_PLAT}/${TARGET_PLAT}-protobuf.cmake)

SET (PROTOBUF_INCLUDE_DIR ${EXTERNAL_DIR}/protobuf/src/)

message("proto_include is ${PROTOBUF_INCLUDE_DIR}")


