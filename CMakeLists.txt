cmake_minimum_required(VERSION 3.10)
project(gresource-generator C)

find_package(LibXml2)

set(PROG_SOURCES main.c writer.c)
set(PROG_NAME gresource-generator)

add_executable(${PROG_NAME} ${PROG_SOURCES})
