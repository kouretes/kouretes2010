include(${KOURETES_DIR}/cmakemodules/bootstrap.cmake)
use(NAOQI-PLUGINS-TOOLS)

add_library(vision vision.cpp vision.h KImageExtractor.cpp KImageExtractor.h) 
use_lib(vision ALCOMMON ALVALUE ALMATH LIBCORE TOOLS LIBVISION OPENCV)
