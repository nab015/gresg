cmake_minimum_required(VERSION 3.0)
project(gresg C)

find_package(LibXml2 REQUIRED)

include_directories(${LIBXML2_INCLUDE_DIR})

set(PROG_SOURCES main.c writer.c argparse.c)
set(PROG_NAME gresg)

add_executable(${PROG_NAME} ${PROG_SOURCES})
target_link_libraries(${PROG_NAME} PUBLIC ${LIBXML2_LIBRARIES})
target_compile_options(${PROG_NAME} PUBLIC -g)

install(TARGETS ${PROG_NAME}
    RUNTIME DESTINATION bin)
