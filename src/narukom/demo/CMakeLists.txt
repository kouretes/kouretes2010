add_executable(ping_local pingpong_local.cpp pinger.cpp ponger.cpp scorekeeper.cpp echo.cpp pingpong.pb.cc)
target_link_libraries(ping_local narukom )

add_executable(synchro_test synchronization_test.cpp fast.cpp slow.cpp ratio.cpp sync.pb.cc)
target_link_libraries( synchro_test narukom)

add_executable(pinger_network ping_network.cpp pinger.cpp pingpong.pb.cc)
target_link_libraries( pinger_network narukom)
add_executable(ponger_network pong_network.cpp ponger.cpp pingpong.pb.cc)
target_link_libraries( ponger_network narukom)
