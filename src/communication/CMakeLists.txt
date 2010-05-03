add_library(communication STATIC communication.cpp led_handler.cpp)
target_link_libraries(communication  robotcontroller)
add_subdirectory(robotcontroller)
use_lib(communication ALCOMMON LIBCORE TOOLS LIBCORE BOOST)
