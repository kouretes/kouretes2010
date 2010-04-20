project(Kouretes2010)

cmake_minimum_required(VERSION 2.6.4)


#


set ( CMAKE_COLOR_MAKEFILE TRUE )
#Getting Current directory
set (KOURETES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
#set (TARGET_PLAT "nao")
#Path to find .cmake files
set (CMAKE_MODULE_PATH "${KOURETES_DIR}/cmakemodules")
IF ( "${TARGET_PLAT}" STREQUAL "")
	set(TARGET_PLAT "nao")
ENDIF( "${TARGET_PLAT}" STREQUAL "")

include(${KOURETES_DIR}/cmakemodules/${TARGET_PLAT}-config.cmake)

include("${KOURETES_DIR}/cmakemodules/deploy.cmake")
message(STATUS "Compiling Kouretes Code for 2010")

SET (LIBRARY_OUTPUT_PATH ${KOURETES_DIR}/build/lib/)
SET (EXECUTABLE_OUTPUT_PATH ${KOURETES_DIR}/build/bin/)


add_subdirectory(src)



