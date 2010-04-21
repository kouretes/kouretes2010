PROJECT(Narukom)
cmake_minimum_required(VERSION 2.6.4)
set ( CMAKE_COLOR_MAKEFILE TRUE )

include_directories( ${NARUKOM_SYSTEM_INCLUDE_DIR}
    ${NARUKOM_PUB_SUB_INCLUDE_DIR}
    ${NARUKOM_NETWORK_INCLUDE_DIR}
    ${NARUKOM_MESSAGES_INCLUDE_DIR}
    ${PROTOBUF_INCLUDE_DIR}
    ${TICPP_INCLUDE_DIR}
    )



#message ("------------------------------------- ${NARUKOM_INCLUDE_DIR} ----------------")
add_definitions(${TARGET_PLAT_DEFINITIONS})


add_library(narukom SHARED narukom.cpp ${NARUKOM_SYSTEM_SRCS}  ${NARUKOM_PUB_SUB_SRCS}  ${NARUKOM_NETWORK_SRCS}
    ${NARUKOM_MESSAGES_SRCS}  )

target_link_libraries(narukom ${PROTOBUF_LIBRARIES} ${ZLIB_LIBRARIES}  ${TICPP_LIBRARIES})
use_lib(narukom TOOLS BOOST LIBCORE PTHREAD BOOST_THREAD)

add_subdirectory(demo)
