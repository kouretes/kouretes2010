include(${KOURETES_DIR}/cmakemodules/bootstrap.cmake)
use(NAOQI-PLUGINS-TOOLS)

add_library(localization STATIC localization.cpp)
target_link_libraries(localization narukom)
use_lib(localization ALCOMMON ALVALUE ALMATH LIBCORE TOOLS)
