include(${KOURETES_DIR}/cmakemodules/bootstrap.cmake)
use(NAOQI-PLUGINS-TOOLS)

add_library(behavior  STATIC behavior.cpp)
target_link_libraries(behavior narukom)
use_lib(behavior ALCOMMON ALVALUE ALMATH LIBCORE TOOLS)
