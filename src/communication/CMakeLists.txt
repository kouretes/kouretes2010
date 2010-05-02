add_library(communication communication.cpp led_handler)
target_link_libraries(communication  robotcontroller)
add_subdirectory(robotcontroller)
use_lib(communication ALCOMMON LIBCORE TOOLS LIBCORE BOOST)
