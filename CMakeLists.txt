cmake_minimum_required(VERSION 2.6)
project(paper soccer)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(Paper Soccer_SRCS Paper Soccer.cpp main.cpp)

qt4_automoc(${Paper Soccer_SRCS})
add_executable(paper soccer ${Paper Soccer_SRCS})
target_link_libraries(paper soccer ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS paper soccer RUNTIME DESTINATION bin)
