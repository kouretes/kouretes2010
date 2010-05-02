

add_library(robotcontroller gamecontroller.cpp robotcontroller.cpp button_checker.cpp)
target_link_libraries(robotcontroller narukom)
use_lib(robotcontroller  LIBCORE TOOLS LIBCORE BOOST TINYXML ALCOMMON ALVALUE)
