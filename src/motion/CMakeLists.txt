include(${KOURETES_DIR}/cmakemodules/bootstrap.cmake)
use(NAOQI-PLUGINS-TOOLS)


add_library(motion STATIC motion_controller.cpp)
target_link_libraries(motion narukom)
use_lib(motion ALCOMMON ALVALUE ALMATH LIBCORE TOOLS)
