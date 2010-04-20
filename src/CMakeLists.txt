

MESSAGE( STATUS "Compiling Communication")

add_subdirectory(communication) 

include("${KOURETES_DIR}/cmakemodules/narukom_ext.cmake")
include("${KOURETES_DIR}/cmakemodules/narukom_srcs.cmake")
include_directories(${NARUKOM_INCLUDE_DIR} ./)

add_subdirectory(narukom)


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


add_library(kouretes_lib krobot.cpp)
add_executable(kouretes_bin krobot.cpp)

target_link_libraries(kouretes_bin narukom motion vision behavior sensors localization)
use_lib(kouretes_bin ALCOMMON ALVALUE ALMATH LIBCORE TOOLS )
