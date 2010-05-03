


include("${KOURETES_DIR}/cmakemodules/narukom_ext.cmake")
include("${KOURETES_DIR}/cmakemodules/narukom_srcs.cmake")
include_directories( ./ 
${NARUKOM_DIR} 
${PROTOBUF_INCLUDE_DIR} 
${TICPP_INCLUDE_DIR})

add_subdirectory(narukom)

add_subdirectory(messages)

MESSAGE( STATUS "Compiling Communication")
add_subdirectory(communication) 


MESSAGE( STATUS "Compiling Vision")
add_subdirectory(vision)

MESSAGE( STATUS "Compiling Motion")
add_subdirectory(motion)

MESSAGE( STATUS "Compiling Sensors")
add_subdirectory(sensors)

MESSAGE( STATUS "Compiling Behavior")
add_subdirectory(behavior)

MESSAGE( STATUS "Localization")
add_subdirectory(localization)

MESSAGE( STATUS "ObstaclesAvoidance")
add_subdirectory(ObstacleAvoidance)

add_library(kouretes_lib krobot.cpp)
add_executable(kouretes_bin krobot.cpp)
target_link_libraries(kouretes_lib messages narukom motion vision behavior sensors localization ObstacleAvoidance  communication)
target_link_libraries(kouretes_bin messages narukom motion vision behavior sensors localization ObstacleAvoidance  communication)
use_lib(kouretes_bin ALCOMMON ALVALUE ALMATH LIBCORE TOOLS )
use_lib(kouretes_lib ALCOMMON ALVALUE ALMATH LIBCORE TOOLS )
